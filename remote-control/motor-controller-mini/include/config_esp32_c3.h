#pragma once

#include <types.h>

/* PWM Outputs */

const short pwm_output_pins[] = { };
const short pwm_outputs_count = sizeof(pwm_output_pins) / sizeof(short);

/* PWM Inputs */

const short pwm_input_pins[] = { 0, 1 };
const short pwm_inputs_count = sizeof(pwm_input_pins) / sizeof(short);

/* ADC Inputs */

const short adc_input_pins[] = { 4, 3 };
const short adc_inputs_count = sizeof(adc_input_pins) / sizeof(short);

/* SBUS */

const short sbus_rx_tx_pins[] = { 20, 21 };

/* Motors */

const static short motors_count = 2;

const motor_drive_mode_t motor_modes[] = {
     motor_drive_mode_t::a_b_en,
     motor_drive_mode_t::a_b_en
};

const motor_pins_t motor_pins[] = {
    { 7,  6, 5, 1, 2, 0 }, //a, b, en, pwm_a, pwm_b, pwm_en
    { 8, 10, 2, 4, 5, 3 }  //b, b, en, pwm_a, pwm_b, pwm_en
};

/* Misc */

const short switch_input_button = 9;

const short led_output = 8;
const short rgb_output = -1;

const bool enable_lcd = true; // i2c 8 and 9