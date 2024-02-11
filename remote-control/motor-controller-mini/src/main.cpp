#ifndef CONFIG_FILE
#error "Config file was not defined"
#endif

#include CONFIG_FILE

#include <network.h>
#include <storage.h>
#include <driver.h>
#include <api.h>
#include <esp32_now.h>
#include <button_input.h>

void on_switch_input(short input) {
    log_w("Button value: %d", input);
}

static button_input_t switch_input { switch_input_button, HIGH, 2000, 0, 2, 0,  &on_switch_input};

void setup() {
  Serial.begin(115200);
  sleep(3);

  storage_init();
  settings_load(motors_config, motors_count);

  driver_init();
  button_input_init(switch_input);

  wifi_init("esp32-motor-ctrl-mini");
  api_init();
  //now_init();
 // web_init();
}

void loop() {
  //ws_loop();
  driver_loop();
  button_input_update(switch_input);
}