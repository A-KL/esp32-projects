#ifndef rc_driver_h
#define rc_driver_h

#include <math.h>
#include <Arduino.h>
#include <sbus.h>

#include <driver_pwm.h>

#include <config_esp32.h>
#include <config_esp32c3_v2.h>

const int MAX_DUTY_CYCLE = (int)(pow(2, MOTOR_RES) - 1);

bfs::SbusRx sbus_rx(&Serial1, sbus_tx_rx_pins[1], sbus_tx_rx_pins[0], true);
bfs::SbusTx sbus_tx(&Serial1, sbus_tx_rx_pins[1], sbus_tx_rx_pins[0], true);
bfs::SbusData sbus_data;

bool near_zero(const int value) {
  return (abs(value) < 30);
}

void init_motors()
{
  for(auto i=0; i<motors_count; ++i)
  {
     switch (motors_config[i].mode)
     {
     case a_b_en:
        init_motors_a_b_en(motor_pins[i]);
        break;

      case a_b:
        init_motors_a_b(motor_pins[i]);
        break;

      case dir_en:
        init_motors_a_en(motor_pins[i]);
        break;
     
     default:
      break;
     }
  }
}

/*
  Control motors via DIR and EN signals.
  Output A is Used as DIR.
*/
void init_motors_a_en(const motor_pins_t& pins)
{
  pinMode(pins.a, OUTPUT);

  ledcSetup(pins.en_channel, MOTOR_FREQ, MOTOR_RES);

  ledcAttachPin(pins.en, pins.en_channel);
}
/*
  Control motors via two PWM signals.
*/
void init_motors_a_b_en(const motor_pins_t& pins)
{
  pinMode(pins.a, OUTPUT);
  pinMode(pins.b, OUTPUT);

  ledcSetup(pins.en_channel, MOTOR_FREQ, MOTOR_RES);

  ledcAttachPin(pins.en, pins.en_channel);
}
/*
  Control motors via two pwm and EN signal.
*/
void init_motors_a_b(const motor_pins_t& pins)
{
  ledcSetup(pins.a_channel, MOTOR_FREQ, MOTOR_RES);
  ledcSetup(pins.b_channel, MOTOR_FREQ, MOTOR_RES);

  ledcAttachPin(pins.a, pins.a_channel);
  ledcAttachPin(pins.b, pins.b_channel);
}

void run_motor_a_b(const byte channel_a, const byte channel_b, const byte channel_en, const int speed) {

  if (near_zero(speed)) {
    ledcWrite(channel_a, 0);
    ledcWrite(channel_b, 0);
  }
  else if (speed > 0) {
    ledcWrite(channel_a, abs(speed));
    ledcWrite(channel_b, 0);
  }
  else {
    ledcWrite(channel_a, 0);
    ledcWrite(channel_b, abs(speed));
  }
}

void run_motor_a_en(const byte pin_a, const byte pin_b, const byte channel_en, const int speed) {

  if (near_zero(speed)) {
    ledcWrite(channel_en, 0);
  }
  else {
    digitalWrite(pin_a, speed > 0 ? HIGH : LOW);

    ledcWrite(channel_en, abs(speed));
  }
}

void run_motor_a_b_en(const byte pin_a, const byte pin_b, const byte channel_en, const int speed) {

  if (near_zero(speed)) {
    digitalWrite(pin_a, LOW);
    digitalWrite(pin_b, LOW);

    ledcWrite(channel_en, 0);
  }
  else {
    digitalWrite(pin_a, speed > 0 ? HIGH : LOW);
    digitalWrite(pin_b, speed > 0 ? LOW : HIGH);

    ledcWrite(channel_en, abs(speed));
  }
}

void driver_init()
{
  for (auto i = 0; i < sizeof(adc_pins); i++)
  {
    pinMode(adc_pins[i], INPUT);
  }
  
  for (auto i = 0; i < sizeof(pwm_pins); i++)
  {
    pinMode(pwm_pins[i], INPUT);
  }
  
  sbus_rx.Begin();

  init_motors();

  attachInterrupt(digitalPinToInterrupt(pwm_pins[0]), TimerInput0, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pwm_pins[1]), TimerInput1, CHANGE);
}

void driver_loop()
{
  int outputs[motors_count];

  if (sbus_rx.Read())
  {
    sbus_data = sbus_rx.data();
    sbus_tx.data(sbus_data);
    sbus_tx.Write();

    for (auto i=0; i<sizeof(motors_count); ++i)
    {
      if (motors_config[i].input_type == sbus)
      {
        auto sbus_index = motors_config[i].input_channel;
        outputs[i] = map(sbus_data.ch[sbus_index], INPUT_SBUS_MIN, INPUT_SBUS_MAX, -MAX_DUTY_CYCLE, MAX_DUTY_CYCLE);
      }
    }
  }

  int pwm_inputs[] = { 
    input_0_pwm.Result(),
    input_1_pwm.Result()
  };

  for (auto i=0; i<sizeof(motors_count); ++i)
  {
    if (motors_config[i].input_type == pwm)
    {
      auto pwm_index = motors_config[i].input_channel;
      auto pwm_value = pwm_inputs[pwm_index];

      if (pwm_value > 1020)
        outputs[i] = map(constrain(pwm_value, INPUT_PWM_MIN, INPUT_PWM_MAX), INPUT_PWM_MIN, INPUT_PWM_MAX, -MAX_DUTY_CYCLE, MAX_DUTY_CYCLE);
      else
        outputs[i] = 0;
    }
    else if (motors_config[i].input_type == adc)
    {
      auto adc_index = motors_config[i].input_channel;
      auto adc_value = analogRead(adc_pins[adc_index]);
      outputs[i] = map(adc_value, INPUT_ADC_MIN, INPUT_ADC_MAX, -MAX_DUTY_CYCLE, MAX_DUTY_CYCLE);
    }
  }

  for (auto i = 0; i < sizeof(outputs); i++)
  {
    RUN_MOTOR(motor_pins[i].a, motor_pins[i].b, motor_pins[i].en, outputs[i]);
  }

  delay(50);
}

#endif