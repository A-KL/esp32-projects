#pragma once

#include <types.h>

/* Inputs */

/* ADC */
const uint8_t adc_input_pins[] = { };

/* PWM */
const uint8_t pwm_input_pins[] = { 23, 22 };

/* SBUS */
const uint8_t sbus_rx_tx_pins[] = { }; //16, 17;
const auto sbus_serial = &Serial1;

/* PS */
const char* ps_controller_mac = "B8:27:EB:DF:B3:FF";

/* Outputs */

/* Servos */
const uint8_t servos_pins[] = { };

const bool servos_inverted[] = {};

/* LEGO */
lego_servo_t lego_servos[] = { };

/* Motors */
motor_config_t motors [] = {
    { dc_mode_t::dir_en, { 15, -1, 04, -1, -1, 0 } }, //a, b, en, pwm_a, pwm_b, pwm_en
    { dc_mode_t::dir_en, { 12, -1, 14, -1, -1, 1 } }  //b, b, en, pwm_a, pwm_b, pwm_en
};