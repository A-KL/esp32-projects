#pragma once

#include <math.h>
#include <Arduino.h>
#include <sbus.h>

#include <driver_pwm.h>
#include <esp32_now.h>

#include <config_esp32.h>
#include <config_esp32_c3.h>

bfs::SbusRx sbus_rx(&Serial1, sbus_rx_tx_pins[0], sbus_rx_tx_pins[1], true);
bfs::SbusTx sbus_tx(&Serial1, sbus_rx_tx_pins[0], sbus_rx_tx_pins[1], true);
bfs::SbusData sbus_data;

bool near_zero(const int value) {
  return (abs(value) < 30);
}

void init_motors_a_en(const motor_pins_t& pins)
{
  pinMode(pins.a, OUTPUT);

  ledcSetup(pins.en_channel, MOTOR_FREQ, MOTOR_RES);

  ledcAttachPin(pins.en, pins.en_channel);
}

void init_motors_a_b_en(const motor_pins_t& pins)
{
  pinMode(pins.a, OUTPUT);
  pinMode(pins.b, OUTPUT);

  ledcSetup(pins.en_channel, MOTOR_FREQ, MOTOR_RES);

  ledcAttachPin(pins.en, pins.en_channel);
}

void init_motors_a_b(const motor_pins_t& pins)
{
  ledcSetup(pins.a_channel, MOTOR_FREQ, MOTOR_RES);
  ledcSetup(pins.b_channel, MOTOR_FREQ, MOTOR_RES);

  ledcAttachPin(pins.a, pins.a_channel);
  ledcAttachPin(pins.b, pins.b_channel);
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

void run_motor_a_b(const byte channel_a, const byte channel_b, const int speed) {

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

void run_motor_a_en(const byte pin_a, const byte channel_en, const int speed) {

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

void run_motor(const motor_pins_t& pins, const motor_config_t& config, const int speed) 
{
     switch (config.mode)
     {
     case a_b_en:
        run_motor_a_b_en(pins.a, pins.b, pins.en_channel, speed);
        break;

      case a_b:
        run_motor_a_b(pins.a_channel, pins.b_channel, speed);
        break;

      case dir_en:
        run_motor_a_en(pins.a, pins.en_channel, speed);
        break;
     
     default:
      break;
     }
}

void driver_init()
{
  for (auto i = 0; i < motors_count; i++)
  {
    pinMode(adc_pins[i], INPUT);
  }
  
  for (auto i = 0; i < motors_count; i++)
  {
    pinMode(pwm_pins[i], INPUT);
  }
  
  sbus_rx.Begin();

  init_motors();

  attachInterrupt(digitalPinToInterrupt(pwm_pins[0]), TimerInput0, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pwm_pins[1]), TimerInput1, CHANGE);
}

void on_esp_now(const channel_t* channels, int channels_count) 
{
    for (auto i=0; i<channels_count; i++) {
        sbus_data.ch[i] = map(message.channels[i].value, INPUT_ESP_NOW_MIN, INPUT_ESP_NOW_MAX, INPUT_SBUS_MIN, INPUT_SBUS_MAX);     
    }
}

void driver_loop()
{
  int outputs[motors_count];

  if (sbus_rx.Read())
  {
    sbus_data = sbus_rx.data();
    sbus_tx.data(sbus_data);
    sbus_tx.Write();

    for (auto i=0; i<motors_count; ++i)
    {
      if (motors_config[i].input_type == sbus)
      {
        auto sbus_index = motors_config[i].input_channel;
        outputs[i] = map(sbus_data.ch[sbus_index], INPUT_SBUS_MIN, INPUT_SBUS_MAX, -MAX_DUTY_CYCLE, MAX_DUTY_CYCLE);
      }
    }
  }

  int pwm_inputs[] = { 
    input_pwm[0].Result(),
    input_pwm[1].Result()
  };

  for (auto i=0; i<motors_count; ++i)
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

  for (auto i = 0; i < motors_count; i++)
  {
    run_motor(motor_pins[i], motors_config[i], outputs[i]);
  }

  delay(50);
}