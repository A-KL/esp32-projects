#pragma once

#include <math.h>
#include <types.h>
#include <Arduino.h>

#include <driver_config.h>

#ifndef MOTOR_PWM_FQC
#define MOTOR_PWM_FQC         2500 // Hz
#endif

#ifndef MOTOR_PWM_RESOLUTION
#define MOTOR_PWM_RESOLUTION  8 // Bit
#endif

#ifndef MOTOR_INPUT_DEAD_ZONE
#define MOTOR_INPUT_DEAD_ZONE 10
#endif

#define MOTOR_DUTY_CYCLE (long)(pow(2, MOTOR_PWM_RESOLUTION) - 1) // with 8 (0-255), 12 (0-4095), or 16 (0-65535) bit resolution

// Utils

inline bool near_zero(const int value) {
  return (abs(value) < MOTOR_INPUT_DEAD_ZONE);
}

// Run motors

void motor_run_a_b(const uint8_t channel_a, const uint8_t channel_b, const int16_t speed) 
{
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

void motor_run_a_en(const uint8_t pin_a, const uint8_t channel_en, const int16_t speed) 
{
  if (near_zero(speed)) {
    ledcWrite(channel_en, 0);
  }
  else {
    digitalWrite(pin_a, speed > 0 ? HIGH : LOW);
    ledcWrite(channel_en, abs(speed));
  }
}

void motor_run_a_b_en(const uint8_t pin_a, const uint8_t pin_b, const uint8_t channel_en, const int16_t speed) 
{
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

void motor_run(const motor_config_t& config, const int16_t speed)
{
#ifdef OUTPUT_MOTOR_DEBUG
    log_d("[MOTOR] (%d) Speed: %d", config.mode, speed);
#endif

    switch (config.mode)
    {
      case a_b_en:
        motor_run_a_b_en(config.pins.a, config.pins.b, config.pins.en_channel, speed);
        break;

      case a_b:
        motor_run_a_b(config.pins.a_channel, config.pins.b_channel, speed);
        break;

      case dir_en:
        motor_run_a_en(config.pins.a, config.pins.en_channel, speed);
        break;
      
      default:
        break;
    }
}

template<short TMin, short TMax>
inline void motor_run(const motor_config_t& config, const int16_t speed)
{
    motor_run(config, map(speed, TMin, TMax, -MOTOR_DUTY_CYCLE, MOTOR_DUTY_CYCLE));
}

// Write

inline void write_motor(const uint8_t index, const int16_t speed)
{
    motor_run(motors[index], speed);
}

template<short TMin, short TMax>
inline void write_motor(const uint8_t index, const int16_t output)
{
    motor_run<TMin, TMax>(motors[index], output);
}

void write_motors(const int16_t* outputs, const uint8_t count)
{
  for (auto i = 0; i<count; ++i) {
    write_motor(i, outputs[i]);
  }
}

template<short TMin, short TMax>
void write_motors(const int16_t* outputs, const uint8_t count)
{
  for (auto i = 0; i<count; ++i) {
    write_motor<TMin, TMax>(i, outputs[i]);
  }
}

// Init motors

void motor_init_a_en(const motor_pins_t& pins)
{
  pinMode(pins.a, OUTPUT);

  ledcSetup(pins.en_channel, MOTOR_PWM_FQC, MOTOR_PWM_RESOLUTION);
  ledcAttachPin(pins.en, pins.en_channel);
}

void motor_init_a_b_en(const motor_pins_t& pins)
{
  pinMode(pins.a, OUTPUT);
  pinMode(pins.b, OUTPUT);

  ledcSetup(pins.en_channel, MOTOR_PWM_FQC, MOTOR_PWM_RESOLUTION);
  ledcAttachPin(pins.en, pins.en_channel);
}

void motor_init_a_b(const motor_pins_t& pins)
{
  ledcSetup(pins.a_channel, MOTOR_PWM_FQC, MOTOR_PWM_RESOLUTION);
  ledcSetup(pins.b_channel, MOTOR_PWM_FQC, MOTOR_PWM_RESOLUTION);

  ledcAttachPin(pins.a, pins.a_channel);
  ledcAttachPin(pins.b, pins.b_channel);
}

void motor_init(const motor_config_t& motor)
{
     switch (motor.mode)
     {
      case a_b_en:
          motor_init_a_b_en(motor.pins);
          break;

      case a_b:
          motor_init_a_b(motor.pins);
          break;

      case dir_en:
          motor_init_a_en(motor.pins);
          break;
     
      default:
          break;
     }
}

void motors_init() 
{
  for (auto i=0; i<motors_count; ++i) {
      motor_init(motors[i]);
      motor_run(motors[i], 0);
  }
}