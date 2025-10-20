#pragma once

#include <types.h>

/* Generic */

#define WIFI_ENABLED
#define HAS_BLE
#define LED_PIN 8

/* Debug */
// #define OUTPUT_SERVOS_DEBUG
// #define INPUT_ESP_NOW_DEBUG

/* Inputs */

/* ADC  */
const uint8_t adc_input_pins[] = { };

/* PWM */
const uint8_t pwm_input_pins[] = { };

/* SBUS */
const uint8_t sbus_rx_tx_pins[] = { 20, 21 };
const auto sbus_serial = &Serial1;

/* Outputs */

/* PWM */
const uint8_t servos_pins[] = { 0, 1, 2, 3, 4, 5 };

/* Motors */
const motor_config_t motors [] = { };

/* LEGO */
static lego_servo_t lego_servos[] = { };