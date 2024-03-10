#pragma once

#include <types.h>

/* PWM Outputs */

const short servos_pins[] = { 0, 1, 2, 3, 4, 5 };
const short servos_count = sizeof(servos_pins) / sizeof(short);

/* PWM Inputs */

const short pwm_input_pins[] = { };
const short pwm_inputs_count = sizeof(pwm_input_pins) / sizeof(short);

/* ADC Inputs */

const short adc_input_pins[] = { };
const short adc_inputs_count = sizeof(adc_input_pins) / sizeof(short);

/* SBUS */

const short sbus_rx_tx_pins[] = { 20, 21 };

/* Motors */

motor_config_t motors [] = { };
const static short motors_count = sizeof(motors) / sizeof(motor_config_t);

/* LEGO */

lego_servo_t lego_servos[] = { };
const int lego_servos_count = sizeof(lego_servos) / sizeof(lego_servo_t);

/* Misc */

const short switch_input_button = 9;

const short led_output = 8;
const short rgb_output = -1;

const bool enable_lcd = true; // i2c 8 and 9