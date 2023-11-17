#ifndef rc_driver_pwm_h
#define rc_driver_pwm_h

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

pwm_input_t input_0_pwm;
pwm_input_t input_1_pwm;

void IRAM_ATTR TimerInput0() {
  input_0_pwm.Capture();
}

void IRAM_ATTR TimerInput1() {
  input_1_pwm.Capture();
}

#endif