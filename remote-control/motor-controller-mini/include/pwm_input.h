#pragma once

#include <Arduino.h>

#include <driver_config.h>

#define INPUT_PWM_MAX   1996//2000
#define INPUT_PWM_MIN   970 //1024
#define INPUT_PWM_ZERO  800 //1020

class pwm_input_t 
{
public:
  void capture() 
  {
    currentTime = micros();
    if (currentTime > startTime) {
      pulses = currentTime - startTime;
      startTime = currentTime;
    }
  }

  int value() 
  {
    if (pulses < 2000) {
      result = pulses;
    }
    return result;
  }
  private:
    volatile unsigned long startTime = 0;
    volatile unsigned long currentTime = 0;
    volatile unsigned long pulses = 0;
    int result = 0;
};

static pwm_input_t input_pwm[pwm_inputs_count];

void IRAM_ATTR timerInputHandler(void* arg) {
  auto input = (pwm_input_t*)arg;
  input->capture();
}

inline void pwm_in_init() {
  for (auto i=0; i < pwm_inputs_count; i++) {
      pinMode(pwm_input_pins[i], INPUT);
      attachInterruptArg(digitalPinToInterrupt(pwm_input_pins[i]), timerInputHandler, &input_pwm[i], CHANGE);
  }
}

inline void pwm_in_deinit() {
    for(auto i=0; i<pwm_inputs_count; i++) {
      detachInterrupt(digitalPinToInterrupt(pwm_input_pins[i]));
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