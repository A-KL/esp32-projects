#pragma once

#ifndef CONFIG_NAME
#error "Config was not defined"
#endif

#define CONFIG_FILE <CONFIG_NAME.h>
#include CONFIG_FILE

#ifndef MOTOR_INPUT_DEAD_ZONE
#define MOTOR_INPUT_DEAD_ZONE 10
#endif

/* Outputs */

/* Servo */
constexpr static uint8_t servos_count = sizeof(servos_pins) / sizeof(uint8_t);

/* Motor */
constexpr static uint8_t motors_count = sizeof(motors) / sizeof(motor_config_t);

/* LEGO */
constexpr static uint8_t lego_servos_count = sizeof(lego_servos) / sizeof(lego_servo_t);

/* Inputs */

/* PWM */
constexpr static uint8_t pwm_inputs_count = sizeof(pwm_input_pins) / sizeof(uint8_t);

/* ADC */
constexpr static uint8_t adc_inputs_count = sizeof(adc_input_pins) / sizeof(uint8_t);

/* SBUS */
constexpr static uint8_t sbus_configured = sizeof(sbus_rx_tx_pins) / sizeof(uint8_t);

/* Helpers */

void log_d_values(const char* label, const int16_t* outputs, const uint8_t count)
{
  char trace[100];
  memset(trace, 0, sizeof(trace) * sizeof(char));
  
  for (auto i = 0; i<count; i++) {
    sprintf(trace, "%s\t%d", trace, outputs[i]);
  }
  log_d("%s (%s)", label, trace);
}

// Utils

inline bool near_zero(const int value) {
  return (abs(value) < MOTOR_INPUT_DEAD_ZONE);
}