#pragma once

#include <Arduino.h>

#include <driver_limits.h>
#include <driver_config.h>

void adc_init() {
      for (auto i = 0; i < adc_inputs_count; i++) {
      pinMode(adc_input_pins[i], INPUT);
  }
}