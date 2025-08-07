#pragma once

#ifndef CONFIG_NAME
#error "Config was not defined"
#endif

#define CONFIG_FILE <CONFIG_NAME.h>
#include CONFIG_FILE

/* Outputs */

/* Servo */
const static uint8_t servos_count = sizeof(servos_pins) / sizeof(uint8_t);

/* Motor */
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

void trace_values(const char* label, const int16_t* outputs, const uint8_t count)
{
  char trace[100];
  memset(trace, 0, sizeof(trace) * sizeof(char));
  
  for (auto i = 0; i<count; ++i) {
    sprintf(trace, "%s\t%d", trace, outputs[i]);
  }
  log_d("%s (%s)", label, trace);
}