#include <Arduino.h>

#ifndef CONFIG_FILE
#error "Config file was not defined"
#endif
#include CONFIG_FILE

#include <network.h>

#include <pwm_input.h>
#include <sbus_input.h>
#include <enow_input.h>

#include <motor_output.h>
#include <servo_output.h>
#include <lego_servo_output.h>

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println(HOSTNAME);
  sleep(2);

  init_wifi();

  pwm_in_init();
  sbus_init();
  enow_init();

  motors_init();
  servos_init();
  lego_servos_init();

  log_i("Initialization...\tDONE");
}

void loop() {
  static int16_t inputs[32];

  static int16_t outputs_motors[motors_count];
  static int16_t outputs_servo[servos_count];
  static int16_t outputs_lego_servo[lego_servos_count];

    // SBUS
  if (sbus_receive(inputs) > 0) 
  {
    // Motors
    //settings_map_inputs(global_config, "sbus", inputs, motor, outputs_motors, motors_count);
    write_motors<INPUT_SBUS_MIN, INPUT_SBUS_MAX>(outputs_motors, motors_count);

    // Servos
    servos_attach(true, servos_count);
   // settings_map_inputs(global_config, "sbus", inputs, servo, outputs_servo, servos_count);
    servos_write<INPUT_SBUS_MIN, INPUT_SBUS_MAX>(outputs_servo, servos_count);

    // Lego Servo
    // settings_map_inputs(global_config, "sbus", inputs, servo, outputs_servo, servos_count);
    lego_servos_write<INPUT_SBUS_MIN, INPUT_SBUS_MAX>(outputs_lego_servo, lego_servos_count);
  }
  else if (enow_receive(inputs) > 0)
  {
    // Motors
   // settings_map_inputs(global_config, "esp_now", inputs, motor, outputs_motors, motors_count);
    write_motors<INPUT_ESP_NOW_MIN, INPUT_ESP_NOW_MAX>(outputs_motors, motors_count);

    // Servos
    servos_attach(true, servos_count);
   // settings_map_inputs(global_config, "esp_now", inputs, servo, outputs_servo, servos_count);
    servos_write<INPUT_ESP_NOW_MIN, INPUT_ESP_NOW_MAX>(outputs_servo, servos_count);
  }
  else if (pwm_receive(inputs) )
  {
    // Motors
    outputs_motors[0] = inputs[0];
    outputs_motors[1] = inputs[2];
    // settings_map_inputs(global_config, "pwm", inputs, motor, outputs_motors, motors_count);
    write_motors<INPUT_PWM_MIN, INPUT_PWM_MAX>(outputs_motors, motors_count);

    // Lego Servo
    outputs_lego_servo[0] = inputs[0];
    outputs_lego_servo[1] = inputs[2];
    // settings_map_inputs(global_config, "pwm", inputs, servo, outputs_servo, servos_count);
    lego_servos_write<INPUT_PWM_MIN, INPUT_PWM_MAX>(outputs_lego_servo, lego_servos_count);
  }
  else // No input
  {
    // Motors
    for (size_t i = 0; i < motors_count; i++) {
      outputs_motors[i] = 0;
    }
    write_motors(outputs_motors, motors_count);

    // Servos
    servos_attach(false);
  }
}