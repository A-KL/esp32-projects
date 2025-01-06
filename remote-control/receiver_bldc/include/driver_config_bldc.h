#pragma once

#define INPUT_SBUS_MIN  200
#define INPUT_SBUS_MAX  1800

#define INPUT_PWM_MIN   1000
#define INPUT_PWM_MAX   2000

#define INPUT_ESP_NOW_MIN   0
#define INPUT_ESP_NOW_MAX   4095

const int pwm_pins[] = { 0, 1, 2, 3, 4, 7 };

const int pwm_channels_count = sizeof(pwm_pins) / sizeof(int);

const int sbus_rx_tx_pins[] = { 20, 21 };

const int sbus_channels_count = 16;