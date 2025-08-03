#pragma once

#include <types.h>

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
const uint8_t servos_pins[] = { 0, 0, 0, 0 };

/* LEGO */
static lego_servo_t lego_servos[] = { 
    { 3, 4, 4, lego_servo_position_t::middle },
    { 3, 4, 4, lego_servo_position_t::middle }
};

/* Motors */
motor_config_t motors [] = {
    { motor_mode_t::a_b_en, { 7,  6, 5, 1, 2, 0 } }, //a, b, en, pwm_a, pwm_b, pwm_en
    { motor_mode_t::a_b_en, { 8, 10, 2, 4, 5, 3 } }  //b, b, en, pwm_a, pwm_b, pwm_en
};

/* Misc */

const uint8_t switch_input_button = 9;

const uint8_t led_output = 8;
const short rgb_output = -1;