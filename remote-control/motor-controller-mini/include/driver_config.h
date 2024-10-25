#pragma once

#ifndef CONFIG_FILE
#error "Config file was not defined"
#endif

#include CONFIG_FILE

/* Outputs */

/* PWM */
const short servos_count = sizeof(servos_pins) / sizeof(short);

/* Motors */
const static short motors_count = sizeof(motors) / sizeof(motor_config_t);

/* LEGO */
const int lego_servos_count = sizeof(lego_servos) / sizeof(lego_servo_t);


/* Inputs */

/* PWM */
const short pwm_inputs_count = sizeof(pwm_input_pins) / sizeof(short);

/* ADC Inputs */
const short adc_inputs_count = sizeof(adc_input_pins) / sizeof(short);