#pragma once

#include <types.h>

/* Debug outputs*/
// #define OUTPUT_SERVOS_DEBUG

/* Debug inputs*/
// #define INPUT_ESP_NOW_DEBUG
// #define INPUT_PS3_RAW_DEBUG
// #define INPUT_PS3_DEBUG

// #define WIFI_ENABLED
#define RGB_LED_PIN 27

/* Inputs */

/* ADC  */
const uint8_t adc_input_pins[] = { };

/* PWM */
const uint8_t pwm_input_pins[] = { };

/* SBUS */
const uint8_t sbus_rx_tx_pins[] = { 3, 1 };
const auto sbus_serial = &Serial1;

/* PS */
const char* ps_controller_mac = "B8:27:EB:DF:B3:FF";

/* Outputs */

/* PWM */
const uint8_t servos_pins[] = { 26, 36, 18, 19, 21, 22, 25 };

/* Motors */
const motor_config_t motors [] = { };

/* LEGO */
static lego_servo_t lego_servos[] = { };
