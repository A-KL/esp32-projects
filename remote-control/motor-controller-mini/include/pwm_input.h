#pragma once

#include <Arduino.h>
#include <driver_config.h>
#include <pwm_in.h>

// 470 (360 degree rotation) 400?
// 800 (180 degree rotation)
#define INPUT_PWM_LOW 470
#define INPUT_PWM_HI  5000

#define INPUT_PWM_MID 1500  // usec
#define INPUT_PWM_MAX 2020 // 1996 // 2481 (360 degree rotation)
#define INPUT_PWM_MIN 1000 // 1020 //  480 (360 degree rotation)

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
  auto pwm_detected = INPUT_PWM_LOW < pwm_value && pwm_value < INPUT_PWM_HI;

  // if (index==1) {
  //  log_d("PWN %u", pwm_value);
  // }

  if (pwm_detected)
  {
    outputs[index] = constrain(pwm_value, INPUT_PWM_MIN, INPUT_PWM_MAX);
  }
  else
  {
    outputs[index] = INPUT_PWM_MID;
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