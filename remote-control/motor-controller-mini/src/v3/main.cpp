#include <Arduino.h>

#ifndef CONFIG_FILE
#error "Config file was not defined"
#endif
#include CONFIG_FILE

#include <ps_input.h>
#include <pwm_input.h>
#include <sbus_input.h>

#include <motor_output.h>
#include <servo_output.h>
#include <lego_servo_output.h>

void setup() {
  Serial.begin(115200);
  Serial.println(HOSTNAME);
  sleep(2);

  pwm_in_init();
  sbus_init();
  ps_init();

  motors_init();
  servos_init();
  lego_servos_init();

  log_i("Initialization...\tDONE");
}

void loop() {
  static int16_t inputs[32];

  static int16_t outputs_motors[motors_count];
  static int16_t outputs_servo[servos_count];
}