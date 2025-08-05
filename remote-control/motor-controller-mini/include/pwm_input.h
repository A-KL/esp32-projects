#pragma once

#include <Arduino.h>
#include <driver_config.h>
#include <pwm_in.h>

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

bool pwm_receive(const uint8_t index, int16_t* output) 
{ 
  if (pwm_inputs_count <= index) {
    return false;
  }

  auto pwm_value = input_pwm[index].value();
  auto pwm_detected = pwm_value > INPUT_PWM_ZERO;

  if (pwm_detected) {
    output[index] = constrain(pwm_value, INPUT_PWM_MIN, INPUT_PWM_MAX);
  }
  else
  {
    output[index] = 0;
  }

  return pwm_detected;
}

uint8_t pwm_receive(int16_t* outputs) 
{
  int count = 0;

  for (auto i=0; i<pwm_inputs_count; i++) {
    if (pwm_receive(i, outputs)) {
      count++;
    }
  }

#ifdef INPUT_PWM_DEBUG
  trace_values("PWM: ", outputs, count);
#endif

  return count;
}