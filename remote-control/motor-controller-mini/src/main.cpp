#include <Arduino.h>

#ifndef CONFIG_FILE
#error "Config file was not defined"
#endif
#include CONFIG_FILE

#include <storage.h>
#include <inputs_queue.h>
#include <network.h>
#include <server.h>
//#include <scheduler.h>
//#include <button_input.h>

#include <sbus_input.h>
#include <ps_input.h>
#include <adc_input.h>
#include <pwm_input.h>
#include <enow_input.h>

#include <motor_output.h>
#include <servo_output.h>
#include <lego_servo_output.h>

// static void on_switch_input(short input) {
//     log_w("Button value: %d", input);
//     short data[16];

//     //send_sbus_data(data, 16);
// }

//static button_input_t switch_input { switch_input_button, LOW, 2000, 0, 3, 0,  &on_switch_input};

static global_config_t global_config;

void setup() {
  Serial.begin(115200);
  Serial.println(HOSTNAME);
  sleep(2);

  storage_init();
  settings_load(global_config);
  init_wifi();

  //button_input_init(switch_input);
  //scheduler_add(300, [](){ send_sbus_data(sbus_data.ch, sbus_data.NUM_CH); });

  // adc_init();
  // pwm_in_init();
  sbus_init();
  ps_init();
  //enow_init();

  motors_init();
  servos_init();

  if (servos_count > 0) {
     servos_start();
  } else {
    servos_stop();
  }

  // lego_servos_init();

  log_i("Initialization...\tDONE");
  
  // wifi_init(HOSTNAME);
  // server_init();
}

void loop() {
  int16_t inputs[32];

  int16_t outputs_motors[motors_count];
  int16_t outputs_servo[servos_count];
  int16_t outputs_servo_lego[lego_servos_count];

  // SBUS
  if (sbus_receive(inputs) > 0) 
  {
    // Motors
    settings_map_inputs(global_config, "sbus", inputs, motor, outputs_motors);
    // outputs_motors[0] = inputs[1];
    // outputs_motors[1] = inputs[2];
    write_motors<INPUT_SBUS_MIN, INPUT_SBUS_MAX>(outputs_motors, motors_count);

    // Servos
    // outputs_servo[0] = outputs[0];
    // outputs_servo[1] = outputs[1];
    // pwm_write<INPUT_SBUS_MIN, INPUT_SBUS_MAX>(outputs_servo, servos_count);

    // Lego
    // outputs_servo_lego[0] = outputs[0];
    // outputs_servo_lego[1] = outputs[1];
    //lego_servos_write<INPUT_SBUS_MIN, INPUT_SBUS_MAX>(outputs_servo_lego, lego_servos_count);
  } 
  else if (ps_receive(inputs) > 0)
  {
    // Motors
    settings_map_inputs(global_config, "ps3", inputs, motor, outputs_motors);
    write_motors<-INPUT_PS_HALF_RANGE, INPUT_PS_HALF_RANGE>(outputs_motors, motors_count);
  }
  else // No input
  {
    // Motors
    memset(outputs_motors, 0, motors_count);
    write_motors(outputs_motors, motors_count);
  }

  delay(50);

  // server_loop();
  // scheduler_loop();
  // button_input_update(switch_input);
}