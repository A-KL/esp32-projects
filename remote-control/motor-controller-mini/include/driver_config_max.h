#pragma once

#include <types.h>

/* Inputs */

/* ADC */
const short adc_input_pins[] = { };

/* PWM */
const short pwm_input_pins[] = { 12, 11, 10, 9 };

/* SBUS */
const short sbus_rx_tx_pins[] = { 44, 43 }; //RX 03, TX 01, uart0
const auto sbus_serial = &Serial1;

/* Outputs */

/* PWM */
const short servos_pins[] = { 7, 14, 15, 16, 17, 18 };

/* LEGO */
lego_servo_t lego_servos[] = {
    { 1, 2, 8 }, //a, b, channel
    { 3, 4, 9 }  //a, b, channel
};

/* Motors */
motor_config_t motors[] = {
    { motor_mode_t::a_b_en, { 40, 39, 41, -1, -1, 6 } }, //a, b, en, pwm_a, pwm_b, pwm_en
    { motor_mode_t::a_b_en, { 42, 45,  6, -1, -1, 7 } }  //b, b, en, pwm_a, pwm_b, pwm_en
};

const int lego_servos_count = sizeof(lego_servos) / sizeof(lego_servo_t);

/* Misc */

const int switch_input_button = 13;

const int led_output = -1;
const int rgb_output = 21;

const bool enable_lcd = true; // i2c 8 and 9