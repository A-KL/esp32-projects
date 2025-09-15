#pragma once

#include <types.h>

/* Debug */

//#define OUTPUT_SERVOS_DEBUG
//#define OUTPUT_MOTORS_DEBUG

// #define INPUT_ESP_NOW_DEBUG
// #define INPUT_PWM_DEBUG

/* Inputs */

/* ADC */
const uint8_t adc_input_pins[] = { };

/* PWM */
const uint8_t pwm_input_pins[] = { 1, 0 };

/* SBUS */
const uint8_t sbus_rx_tx_pins[] = { 20, 21 };

const auto sbus_serial = &Serial1;

/* Outputs */

/* PWM */
const uint8_t servos_pins[] = { 4, 3 };

/* LEGO */
static lego_servo_t lego_servos[] = { };

/* Motors */
motor_config_t motors [] = {
    { dc_mode_t::a_b_en, { 7,  6, 5, -1, -1, 0 } }, //a, b, en, pwm_a, pwm_b, pwm_en
    { dc_mode_t::a_b_en, { 8, 10, 2, -1, -1, 1 } }  //b, b, en, pwm_a, pwm_b, pwm_en
};

/* Misc */

// const short switch_input_button = 9;
// const short led_output          = 8;
// const short rgb_output          = -1;