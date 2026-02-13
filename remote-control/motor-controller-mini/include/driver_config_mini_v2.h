#pragma once

#include <types.h>

/* Debug */

// #define OUTPUT_SERVOS_DEBUG

// #define INPUT_ESP_NOW_DEBUG
// #define INPUT_PWM_DEBUG

/* Inputs */

/* ADC */
const uint8_t adc_input_pins[] = { };

/* PWM */
const uint8_t pwm_input_pins[] = { 17, 18, 15, 16 };

/* SBUS */
const uint8_t sbus_rx_tx_pins[] = { 44, 43 };
const auto sbus_serial = &Serial1;

/* Outputs */

/* PWM */
const uint8_t servos_pins[] = {};

/* LEGO */
static lego_servo_t lego_servos[] = { 
    { 5, 6, 0, lego_servo_position_t::middle },
    { 3, 4, 1, lego_servo_position_t::middle }
};

/* Motors */
motor_config_t motors [] = {
    { dc_mode_t::a_b_en, {  8,  9,  7, -1, -1, 2 } }, //a, b, en, pwm_a, pwm_b, pwm_en
    { dc_mode_t::a_b_en, { 10, 11, 12, -1, -1, 3 } }  //b, b, en, pwm_a, pwm_b, pwm_en
};

void map_inputs_outputs(    
    const input_type_t input_type,
    const int16_t* inputs, 
    const output_type_t output_type, 
    int16_t* outputs, 
    const uint8_t inputs_count) 
{
    switch (input_type)
    {
        case input_type_t::pwm:

            if (output_type == output_type_t::dc) {
                outputs[0] = inputs[0];
                outputs[1] = inputs[1];
            } else if (output_type == output_type_t::servo_lego) {
                outputs[0] = inputs[2];
                outputs[1] = inputs[3];
            }
            break;

        case input_type_t::sbus:

            if (output_type == output_type_t::dc) {
                outputs[0] = inputs[0];
                outputs[1] = inputs[1];
            } else if (output_type == output_type_t::servo_lego) {
                outputs[0] = inputs[2];
                outputs[1] = inputs[3];
            }
            break;
        
        default:
            break;
    }
}