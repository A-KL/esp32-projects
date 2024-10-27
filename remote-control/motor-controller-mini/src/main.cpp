#include <Arduino.h>

#ifndef CONFIG_FILE
#error "Config file was not defined"
#endif
#include CONFIG_FILE

#include <storage.h>
#include <scheduler.h>
#include <driver.h>
#include <server.h>
#include <button_input.h>

#include <motor.h>
#include <pwm_output.h>
#include <lego_servo.h>

// static void on_switch_input(short input) {
//     log_w("Button value: %d", input);
//     short data[16];

//     //send_sbus_data(data, 16);
// }

//static button_input_t switch_input { switch_input_button, LOW, 2000, 0, 3, 0,  &on_switch_input};

void setup() {
  Serial.begin(115200);
  Serial.println(HOSTNAME);
  sleep(3);

  storage_init();

  //button_input_init(switch_input);
  //scheduler_add(300, [](){ send_sbus_data(sbus_data.ch, sbus_data.NUM_CH); });

  driver_init();
  
  // wifi_init(HOSTNAME);
  // server_init();
  // enow_init();
}

void loop() {
  //server_loop();
  //driver_loop();

  int16_t outputs[32];

  int16_t outputs_motors[motors_count];
  int16_t outputs_servo[servos_count];
  int16_t outputs_servo_lego[lego_servos_count];

  // SBUS
  auto channels = sbus_receive(outputs);
  if (channels > 0) 
  {
    // Motors
    outputs_motors[0] = outputs[1];
    outputs_motors[1] = outputs[2];
    write_motors<INPUT_SBUS_MIN, INPUT_SBUS_MAX>(outputs_motors, 2);

    // Servos
    // outputs_servo[0] = outputs[0];
    // outputs_servo[1] = outputs[1];
    // pwm_write<INPUT_SBUS_MIN, INPUT_SBUS_MAX>(outputs_servo, 2);

    // Lego
    outputs_servo_lego[0] = outputs[0];
    outputs_servo_lego[1] = outputs[1];
    //lego_servos_write<INPUT_SBUS_MIN, INPUT_SBUS_MAX>(outputs_servo_lego, 2);
  } 
  else if(ps_receive(outputs) > 0)
  {
    // Motors
    outputs_motors[0] = outputs[1];
    outputs_motors[1] = outputs[3];
    write_motors<INPUT_PS_HALF_RANGE, -INPUT_PS_HALF_RANGE>(outputs_motors, 2);
  }
  else // No input
  {
    // Motors
    memset(outputs_motors, 0, motors_count);
    write_motors(outputs_motors, motors_count);
  }

  delay(50);

  // scheduler_loop();
  // button_input_update(switch_input);
}