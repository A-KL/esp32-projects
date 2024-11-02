#pragma once

#include <Arduino.h>
#include <ResponsiveAnalogRead.h>
#include <driver_config.h>

#define INPUT_ADC_MAX   4095
#define INPUT_ADC_MIN   0

static ResponsiveAnalogRead analog[adc_inputs_count];

inline void adc_init() {
    for (auto i = 0; i < adc_inputs_count; i++) {
      analog[i].begin(adc_input_pins[i], true);
  }
}

uint8_t adc_receive(int16_t* outputs) 
{
  bool changed = false;

  for (auto i = 0; i < adc_inputs_count; i++) {
    outputs[i] = analog[i].getValue();
    changed |= analog[i].hasChanged();
  }

#ifdef INPUT_ADC_DEBUG
  trace_values("ADC: ", outputs, adc_inputs_count);
#endif

  return changed;
}

void adc_loop() {
  for (auto i = 0; i < adc_inputs_count; i++) {
      analog[i].update();
  }
}