#pragma once

#include <Arduino.h>

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

// void IRAM_ATTR TimerInput0() {
//   input_pwm[0].Capture();
// }

// void IRAM_ATTR TimerInput1() {
//   input_pwm[1].Capture();
// }

// void IRAM_ATTR TimerInput2() {
//   input_pwm[2].Capture();
// }

// void IRAM_ATTR TimerInput3() {
//   input_pwm[3].Capture();
// }

void IRAM_ATTR TimerInputHandler(void* arg) {
  // auto index = *(short*)arg;
  // input_pwm[index].Capture();
  auto input = (pwm_input_t*)arg;
  input->Capture();
}

inline void pwm_in_init() {
  for(auto i=0; i<pwm_inputs_count; i++) {
      attachInterruptArg(digitalPinToInterrupt(pwm_input_pins[i]), TimerInputHandler, &input_pwm[i], CHANGE);
  }
  // attachInterrupt(digitalPinToInterrupt(pwm_input_pins[i]), TimerInput0, CHANGE);
  // attachInterrupt(digitalPinToInterrupt(pwm_input_pins[1]), TimerInput1, CHANGE);
}

inline void pwm_in_deinit() {
    for(auto i=0; i<pwm_inputs_count; i++) {
      detachInterrupt(digitalPinToInterrupt(pwm_input_pins[i]));
  }      
}
