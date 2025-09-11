#pragma once

#include <Arduino.h>
#include <driver_config.h>
#include <pwm_in.h>

#define INPUT_PWM_MAX  1996 // 2000
#define INPUT_PWM_MIN   970 // 1024
#define INPUT_PWM_ZERO  800 // 1020

constexpr static unsigned INPUT_PWM_MID = (INPUT_PWM_MIN + (INPUT_PWM_MAX - INPUT_PWM_MIN) / 2.0);

static pwm_input_t input_pwm[pwm_inputs_count];

inline void pwm_in_init() {
  for (auto i=0; i < pwm_inputs_count; i++) {
      input_pwm[i].init(pwm_input_pins[i]);
  }
}

inline void pwm_in_deinit() {
    for(auto i=0; i<pwm_inputs_count; i++) {
      input_pwm[i].deinit();
  }      
}

bool pwm_receive(const uint8_t index, int16_t* outputs) 
{ 
  if (pwm_inputs_count <= index) {
    return false;
  }

  auto pwm_value = input_pwm[index].value();
  auto pwm_detected = pwm_value > INPUT_PWM_ZERO && pwm_value < 5000;

  //log_d("PWN %u", pwm_value);

  if (pwm_detected)
  {
    outputs[index] = constrain(pwm_value, INPUT_PWM_MIN, INPUT_PWM_MAX);
  }
  else
  {
    outputs[index] = 0;// INPUT_PWM_MID;
  }

  return pwm_detected;
}

uint8_t pwm_receive(int16_t* outputs) 
{
  uint8_t count = 0;
  for (auto i=0; i<pwm_inputs_count; i++) {
    if (pwm_receive(i, outputs)) {
      count++;
    }
  }

#ifdef INPUT_PWM_DEBUG
  // log_d_values("PWM: ", outputs, count);
  log_d("PWM: (%d, %d)", outputs[0], outputs[1]);
#endif

  return count;
}