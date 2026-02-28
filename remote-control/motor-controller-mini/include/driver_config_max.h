#pragma once

#include <types.h>

/* Debug */

// #define OUTPUT_SERVO_DEBUG

/* Inputs */

/* ADC */
const uint8_t adc_input_pins[] = { };

/* PWM */
const uint8_t pwm_input_pins[] = { 12, 11, 10, 9 };

/* SBUS */
const uint8_t sbus_rx_tx_pins[] = { 44, 43 }; //RX 03, TX 01, uart0
const auto sbus_serial = &Serial1;

/* Outputs */

/* PWM */
const uint8_t servos_pins[] = { 7, 14, 15, 16 }; //, 17, 18

const bool servos_inverted[] = { false, false, false, false };

/* LEGO */
lego_servo_t lego_servos[] = {
    { 1, 2, 4 }, //a, b, channel
    { 3, 4, 5 }  //a, b, channel
};

/* Motors */
motor_config_t motors[] = {
    { dc_mode_t::a_b_en, { 40, 39, 41, -1, -1, 6 } }, //a, b, en, pwm_a, pwm_b, pwm_en
    { dc_mode_t::a_b_en, { 42, 45,  6, -1, -1, 7 } }  //b, b, en, pwm_a, pwm_b, pwm_en
};

/* Misc */

// const uint8_t switch_input_button = 13;

// const uint8_t led_output = -1;
// const uint8_t rgb_output = 21;

// const bool enable_lcd = true; // i2c 8 and 9