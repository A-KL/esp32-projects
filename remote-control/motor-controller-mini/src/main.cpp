#ifndef CONFIG_FILE
#error "Config file was not defined"
#endif
#include CONFIG_FILE

#include <network.h>
#include <storage.h>
#include <motors.h>
#include <driver.h>
#include <api.h>
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

  storage_init();
  settings_load(motors_config, motors_count);
  settings_load_v2(global_config);

  driver_init();
  button_input_init(switch_input);

  wifi_init(HOSTNAME);
  api_init();
  web_init();
  //now_init();

}

void loop() {
  //ws_loop();
  driver_loop();
  button_input_update(switch_input);
}