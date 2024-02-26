#ifndef CONFIG_FILE
#error "Config file was not defined"
#endif
#include CONFIG_FILE

#include <scheduler.h>
#include <storage.h>
#include <motors.h>
#include <lego_servo.h>

#include <driver.h>
#include <server.h>
#include <esp32_now.h>
#include <button_input.h>

static void on_switch_input(short input) {
    log_w("Button value: %d", input);
    short data[16];

    send_sbus_data(data, 16);
}

static button_input_t switch_input { switch_input_button, LOW, 2000, 0, 3, 0,  &on_switch_input};

void setup() {
  Serial.begin(115200);
  sleep(3);

  scheduler_add(300, [](){ send_sbus_data(sbus_data.ch, sbus_data.NUM_CH); });

  storage_init();
  settings_load(motors_config, motors_count);
  settings_load_v2(global_config);

  driver_init();
  button_input_init(switch_input);

  wifi_init(HOSTNAME);
  server_init();
  //now_init();
}

void loop() {
  server_loop();
  driver_loop();
  scheduler_loop();

  button_input_update(switch_input);
}