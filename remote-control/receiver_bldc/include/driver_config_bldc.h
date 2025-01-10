#pragma once

#define INPUT_SBUS_MIN  200
#define INPUT_SBUS_MAX  1800

#define INPUT_PWM_MIN   1000
#define INPUT_PWM_MAX   2000

#define INPUT_ESP_NOW_MIN   0
#define INPUT_ESP_NOW_MAX   4095

/* PS */
const char* ps_controller_mac = "b8:27:eb:df:b3:ff";

/* SBUS */
const uint8_t sbus_rx_tx_pins[] = { 16, 17 };
const auto sbus_serial = &Serial1;

/* Motors */
// const uint8_t motor_dir [] = { 25, 26 };
// const uint8_t motor_speed [] = { 33, 27 };
// const uint8_t motor_pwm_ch [] = { 0, 1 };
// const uint8_t motor_stop [] = { 0, 1 };
// const uint8_t motor_brake [] = { 0, 1 };

// const static uint8_t motors_count = sizeof(motor_dir) / sizeof(uint8_t);