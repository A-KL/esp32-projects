#pragma once

#include <Arduino.h>

#include <driver_config.h>

#define INPUT_PWM_MAX   1996//2000
#define INPUT_PWM_MIN   970 //1024
#define INPUT_PWM_ZERO  800 //1020

typedef struct {
  volatile unsigned long startTime = 0;
  volatile unsigned long currentTime = 0;
  volatile unsigned long pulses = 0;
  int result = 0;

  void Capture() {
    currentTime = micros();
    if (currentTime > startTime) {
      pulses = currentTime - startTime;
      startTime = currentTime;
    }
  }
  int Result() {
    if (pulses < 2000) {
      result = pulses;
    }
    return result;
  }
} pwm_input_t;

static pwm_input_t input_pwm[pwm_inputs_count];

void IRAM_ATTR TimerInputHandler(void* arg) {
  auto input = (pwm_input_t*)arg;
  input->Capture();
}

inline void pwm_in_init() {
  for (auto i=0; i < pwm_inputs_count; i++) {
      pinMode(pwm_input_pins[i], INPUT);
      attachInterruptArg(digitalPinToInterrupt(pwm_input_pins[i]), TimerInputHandler, &input_pwm[i], CHANGE);
  }
}

inline void pwm_in_deinit() {
    for(auto i=0; i<pwm_inputs_count; i++) {
      detachInterrupt(digitalPinToInterrupt(pwm_input_pins[i]));
  }      
}
