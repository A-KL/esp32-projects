#pragma once

#include <filters.hpp>

InputFilter* inputs[CHANNELS_COUNT];

void inputs_init() 
{
  auto index = 0;

  for(auto i=0; i<adc_input_pins_size; ++i) {
    inputs[index++] = new SmoothingFilter(adc_input_pins[i]); //&adc_inputs[i];
  }

  for(auto i=0; i<sw_three_input_pins_size; i+=2) {
    inputs[index+=2] = new ThreeWaySwitchFilter(sw_three_input_pins[i], sw_three_input_pins[i+1]);// &sw_three_inputs[i];
  }

  for(auto i=0; i<sw_two_input_pins_size; ++i) {
    inputs[index++] = new TwoWaySwitchFilter(sw_two_input_pins[i]); // &sw_two_inputs[i];
  }
}

void drop_inputs()
{
    for (auto i=0; i<CHANNELS_COUNT; ++i) {
        free(inputs[i]);
    }
}