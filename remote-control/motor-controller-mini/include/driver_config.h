#pragma once

#ifndef CONFIG_FILE
#error "Config file was not defined"
#endif

#include CONFIG_FILE

/* Outputs */

/* PWM */
const uint8_t servos_count = sizeof(servos_pins) / sizeof(uint8_t);

/* Motors */
const static uint8_t motors_count = sizeof(motors) / sizeof(motor_config_t);

/* LEGO */
const static uint8_t lego_servos_count = sizeof(lego_servos) / sizeof(lego_servo_t);

/* Inputs */

/* PWM */
const static uint8_t pwm_inputs_count = sizeof(pwm_input_pins) / sizeof(uint8_t);

/* ADC */
const static uint8_t adc_inputs_count = sizeof(adc_input_pins) / sizeof(uint8_t);

/* SBUS */
const static uint8_t sbus_configured = sizeof(sbus_rx_tx_pins) / sizeof(uint8_t);