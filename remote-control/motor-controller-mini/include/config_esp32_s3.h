#pragma once

#include <types.h>

/* PWM Outputs */

const short servos_pins[] = { 0, 1, 2, 3, 4, 7 };
const short servos_count = sizeof(servos_pins) / sizeof(short);

/* PWM Inputs */

const short pwm_input_pins[] = { 0, 1 };
const short pwm_inputs_count = sizeof(pwm_input_pins) / sizeof(short);

/* ADC Inputs */

const short adc_input_pins[] = { 4, 3 };
const short adc_inputs_count = sizeof(adc_input_pins) / sizeof(short);

/* SBUS */

const short sbus_rx_tx_pins[] = { 20, 21 }; //RX 03, TX 01, uart0

/* Motors */

motor_config_t motors[] = {
    { motor_mode_t::a_b_en, { 7,  6, 5, 1, 2, 0 } }, //a, b, en, pwm_a, pwm_b, pwm_en
    { motor_mode_t::a_b_en, { 8, 10, 2, 4, 5, 3 } }  //b, b, en, pwm_a, pwm_b, pwm_en
};
const static short motors_count = sizeof(motors) / sizeof(motor_config_t);

/* LEGO */

lego_servo_t lego_servos[] = {
    { 7,  6, 4 }, //a, b, channel
    { 8, 10, 5 }  //a, b, channel
};

const int lego_servos_count = sizeof(lego_servos) / sizeof(lego_servo_t);

/* Misc */

const int switch_input_button = 9;

const int led_output = -1;
const int rgb_output = 21;

const bool enable_lcd = true; // i2c 8 and 9