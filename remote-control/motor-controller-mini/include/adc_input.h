#pragma once

#include <Arduino.h>

#include <driver_limits.h>
#include <driver_config.h>

#define INPUT_ADC_MAX   4095
#define INPUT_ADC_MIN   0

void trace_values(const char* label, const uint16_t* outputs, const uint8_t count)
{
  char trace[100];
  for (auto i = 0; i<count; ++i) {
    sprintf(trace, "%s\t%d", trace, outputs[i]);
  }
  log_d("%s (%s)", label, trace);
}

inline void adc_init() {
    for (auto i = 0; i < adc_inputs_count; i++) {
      pinMode(adc_input_pins[i], INPUT);
  }
}

template<long int TMin, long int TMax>
bool adc_read(const uint8_t index, int16_t &result)
{
    if (adc_inputs_count <= index) {
      return false;
    }

  // TODO: Update this
    auto adc_index = index;// motors_config[index].input_channel;
    auto adc_value = analogRead(adc_input_pins[adc_index]);

    result = map(adc_value, INPUT_ADC_MIN, INPUT_ADC_MAX, TMin, TMax);
    return true;
}

template<long int TMin, long int TMax>
bool adc_read(int16_t* outputs) 
{
  if (adc_inputs_count = 0) {
    return false;
  }

  for (auto i=0; i<adc_inputs_count; i++) {
    if (!adc_read<TMin, TMax>(i, outputs[i])) {
      return false;
    }
  }

#ifdef INPUT_ADC_DEBUG
  trace_values("[ADC] Values: ", outputs, adc_inputs_count);
#endif

  return true;
}