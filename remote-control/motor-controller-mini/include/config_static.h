#pragma once

#include <types.h>

void config_init() {
}

void config_map_inputs(
    const input_type_t input_type,
    const int16_t* inputs, 
    const output_type_t output_type, 
    int16_t* outputs, 
    const uint8_t inputs_count)
{
    for (auto i = 0; i < inputs_count; i++) {
       outputs[i] = inputs[i];
    }
}