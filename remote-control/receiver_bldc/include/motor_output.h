#pragma once

#include <Arduino.h>

#ifndef MOTOR_INPUT_DEAD_ZONE
#define MOTOR_INPUT_DEAD_ZONE 10
#endif

#ifndef MOTOR_PWM_FQC
#define MOTOR_PWM_FQC         10000 // Hz
#endif

#ifndef MOTOR_PWM_RESOLUTION
#define MOTOR_PWM_RESOLUTION  8 // Bit
#endif

#ifndef INPUT_MOTOR_MAX_LIMIT
#define INPUT_MOTOR_MAX_LIMIT 1
#endif

const static int motor_duty_cycle = (255 * INPUT_MOTOR_MAX_LIMIT); // with 8 (0-255), 12 (0-4095), or 16 (0-65535) bit resolution

// Utils

inline bool near_zero(const int value) {
  return (abs(value) < MOTOR_INPUT_DEAD_ZONE);
}

// Run motors

void motor_write_a_en(const uint8_t pin_a, const uint8_t channel_en, const int16_t speed) 
{
  if (near_zero(speed)) {
    ledcWrite(channel_en, 0);
  }
  else {
    digitalWrite(pin_a, speed > 0 ? HIGH : LOW);
    ledcWrite(channel_en, abs(speed));
  }
}

template<short TMin, short TMax>
void motor_write_a_en(const uint8_t pin_a, const uint8_t channel_en, const int16_t speed)
{
  motor_write_a_en(pin_a, channel_en, map(constrain(speed, TMin, TMax), TMin, TMax, -motor_duty_cycle, motor_duty_cycle));
}

// Init motors

void motor_init_a_en(const uint8_t pin_a, const uint8_t pin_en, const uint8_t channel_en = 0)
{
  pinMode(pin_a, OUTPUT);

  ledcSetup(channel_en, MOTOR_PWM_FQC, MOTOR_PWM_RESOLUTION);
  ledcAttachPin(pin_en, channel_en);
}

void motors_init()
{
    for (auto i = 0; i < motors_count; i++) {
    motor_init_a_en(motor_dir[i], motor_speed[i], motor_pwm_ch[i]);
  }
}