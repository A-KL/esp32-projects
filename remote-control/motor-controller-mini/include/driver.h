#pragma once

#include <math.h>
#include <Arduino.h>
#include <sbus.h>

#include <config_esp32.h>
#include <driver_pwm.h>
#include <esp32_now.h>

bfs::SbusRx sbus_rx(&Serial1, sbus_rx_tx_pins[0], sbus_rx_tx_pins[1], true);
bfs::SbusTx sbus_tx(&Serial1, sbus_rx_tx_pins[0], sbus_rx_tx_pins[1], true);
bfs::SbusData sbus_data;

void on_esp_now(const channel_t* channels, int channels_count) {

  for (auto i=0; i<channels_count; i++) {
      sbus_data.ch[i] = map(message.channels[i].value, INPUT_ESP_NOW_MIN, INPUT_ESP_NOW_MAX, INPUT_SBUS_MIN, INPUT_SBUS_MAX);     
  }
  sbus_tx.data(sbus_data);
  sbus_tx.Write();
}

void driver_init() {

  for (auto i = 0; i < adc_inputs_count; i++) {
      pinMode(adc_input_pins[i], INPUT);
  }
  
  for (auto i = 0; i < pwm_inputs_count; i++) {
      pinMode(pwm_input_pins[i], INPUT);
  }

  for (auto i = 0; i < pwm_outputs_count; i++) {
      ledcSetup(i, SERVO_FREQ, SERVO_RES);
      ledcAttachPin(pwm_output_pins[i], i);
  }

  for (auto& servo : lego_servos) {
      lego_servo_init(servo);
  }

  attachInterrupt(digitalPinToInterrupt(pwm_input_pins[0]), TimerInput0, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pwm_input_pins[1]), TimerInput1, CHANGE);
  
  sbus_rx.Begin();
  sbus_tx.Begin();

  init_motors();
}

inline bool read_sbus(int outputs[], const int count) 
{
  if (!sbus_rx.Read()) 
  {
     return false;
  }

  sbus_data = sbus_rx.data();
  
  for (auto i=0; i<count; ++i)
  {
      auto sbus_index = motors_config[i].input_channel;
      outputs[i] = map(sbus_data.ch[sbus_index], INPUT_SBUS_MIN, INPUT_SBUS_MAX, -MOTOR_DUTY_CYCLE, MOTOR_DUTY_CYCLE);
  }

  sbus_tx.data(sbus_data);
  sbus_tx.Write();

  return true;
}

inline bool read_pwm(int outputs[], const int count) 
{ 
  int pwm_inputs[] = { 
    input_pwm[0].Result(),
    input_pwm[1].Result()
  };
  
  for (auto i=0; i<count; ++i)
  {
    auto pwm_index = motors_config[i].input_channel;
    auto pwm_value = pwm_inputs[pwm_index];

    //log_d("PWM IN: %d  %d", pwm_inputs[0], pwm_inputs[1]);

    if (pwm_value > INPUT_PWM_ZERO)
      outputs[i] = map(constrain(pwm_value, INPUT_PWM_MIN, INPUT_PWM_MAX), INPUT_PWM_MIN, INPUT_PWM_MAX, -MOTOR_DUTY_CYCLE, MOTOR_DUTY_CYCLE);
    else
      outputs[i] = 0;
  }
  return true;
}

inline bool read_adc(int outputs[], const int count) 
{
  for (auto i=0; i<count; ++i)
  {
      auto adc_index = motors_config[i].input_channel;
      auto adc_value = analogRead(adc_input_pins[adc_index]);

      log_d("ADC IN %d: %d", i, adc_value);

      outputs[i] = map(adc_value, INPUT_ADC_MIN, INPUT_ADC_MAX, -MOTOR_DUTY_CYCLE, MOTOR_DUTY_CYCLE);
  }
  return true;
}

void write_motors(int outputs[], const int count)
{
  for (auto i = 0; i < count; i++)
  {
    run_motor(motor_pins[i], motors_config[i], outputs[i]);
    log_d("MOTOR OUT %d: %d", i, outputs[i]);
  }
}

void driver_loop()
{
  int outputs[motors_count];

  if (read_sbus(outputs, motors_count))
  {
    write_motors(outputs, motors_count);
    return;
  }

  if (read_pwm(outputs, motors_count))
  {
    write_motors(outputs, motors_count);
    return;
  }

  if (read_adc(outputs, motors_count))
  {
    write_motors(outputs, motors_count);
    return;
  }
}