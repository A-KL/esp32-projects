#pragma once

#include <types.h>

/* Inputs */

/* ADC */
const uint8_t adc_input_pins[] = { };

/* PWM */
const uint8_t pwm_input_pins[] = { };

/* SBUS */
const uint8_t sbus_rx_tx_pins[] = { 16, 17 };
const auto sbus_serial = &Serial1;

/* PS */
const char* ps_controller_mac = "B8:27:EB:DF:B3:FF";

/* Outputs */

/* Servos */
const uint8_t servos_pins[] = { };

/* LEGO */
lego_servo_t lego_servos[] = { };

/* Motors */
motor_config_t motors [] = {
    { motor_mode_t::dir_en, { 25, -1, 33, -1, -1, 0 } }, //a, b, en, pwm_a, pwm_b, pwm_en
    { motor_mode_t::dir_en, { 26, -1, 27, -1, -1, 1 } }  //b, b, en, pwm_a, pwm_b, pwm_en
};

/* Misc */

const uint8_t switch_input_button = 4;

// const uint8_t led_output = 8;
// const short rgb_output = -1;