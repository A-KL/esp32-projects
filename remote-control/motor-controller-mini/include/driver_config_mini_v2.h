#pragma once

#include <types.h>

/* Debug */

// #define OUTPUT_SERVOS_DEBUG
// #define INPUT_ESP_NOW_DEBUG
#define INPUT_PWM_DEBUG

/* Inputs */

/* ADC */
const uint8_t adc_input_pins[] = { };

/* PWM */
const uint8_t pwm_input_pins[] = { 12, 13 };

/* SBUS */
const uint8_t sbus_rx_tx_pins[] = { 44, 43 };
const auto sbus_serial = &Serial1;

/* Outputs */

/* PWM */
const uint8_t servos_pins[] = { 3, 4, 5, 6 };

/* LEGO */
static lego_servo_t lego_servos[] = { 
    { 1, 2, 0, lego_servo_position_t::middle },
    { 7, 8, 1, lego_servo_position_t::middle }
};

/* Motors */
motor_config_t motors [] = {
    { dc_mode_t::a_b_en, {  9, 10, 11, -1, -1, 2 } }, //a, b, en, pwm_a, pwm_b, pwm_en
    { dc_mode_t::a_b_en, { 18, 17, 16, -1, -1, 3 } }  //b, b, en, pwm_a, pwm_b, pwm_en
};

/* Misc */
// const short input_button_switch = -1;
// const short output_led          = 7;
// const short output_led_rgb      = -1;