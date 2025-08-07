#pragma once

#include <Arduino.h>

void timerInputHandler(void* arg);

class pwm_input_t 
{
public:
  void init(const int pin) {
    _pin = pin;
    pinMode(_pin, INPUT_PULLDOWN);
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
