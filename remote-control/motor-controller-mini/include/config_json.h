#pragma once

#include <types.h>
#include <storage.h>

static global_config_t global_config;

void config_init() {
    storage_init();
    settings_load(global_config);
}

void config_map_inputs(
    const input_type_t input_type, 
    const int16_t* inputs, 
    const output_type_t output_type, 
    int16_t* outputs, 
    const uint8_t inputs_count)
{
    settings_map_inputs(global_config, input_type, inputs, output_type, outputs, inputs_count);
}