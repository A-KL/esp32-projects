#pragma once

#include <Arduino.h>

#define INPUT_PWM_MAX  1996 // 2000
#define INPUT_PWM_MIN   970 // 1024
#define INPUT_PWM_ZERO  800 // 1020

void timerInputHandler(void* arg);

class pwm_input_t 
{
public:
  void init(const int pin) {
    _pin = pin;
    pinMode(_pin, INPUT);
    attachInterruptArg(digitalPinToInterrupt(_pin), timerInputHandler, this, CHANGE);
  }

  void deinit() {
    detachInterrupt(digitalPinToInterrupt(_pin));
  }

  void capture() {
    if (digitalRead(_pin) == HIGH) {
        // start measuring
        pulseInTimeBegin = micros();
      } else {
        // stop measuring
        pulseInTimeEnd = micros();
        newPulseDurationAvailable = true;
      }
  }

  unsigned long value() {
    if (newPulseDurationAvailable) {
      newPulseDurationAvailable = false;
      unsigned long pulseDuration = pulseInTimeEnd - pulseInTimeBegin;
      return pulseDuration;
    }
    return 0;
  }

  private:
    volatile unsigned long pulseInTimeBegin = micros();
    volatile unsigned long pulseInTimeEnd = micros();
    volatile bool newPulseDurationAvailable = false;
    int _pin;
};

void IRAM_ATTR timerInputHandler(void* arg) {
  auto input = (pwm_input_t*)arg;
  input->capture();
}
