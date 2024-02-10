#ifndef CONFIG_FILE
#error "Config file was not defined"
#endif

#include CONFIG_FILE

#include <network.h>
#include <storage.h>
#include <driver.h>
#include <api.h>
#include <esp32_now.h>

void setup() {
  Serial.begin(115200);
  sleep(3);

  storage_init();
  settings_load(motors_config, motors_count);

  driver_init();

  wifi_init("esp32-motor-ctrl-mini");
  //api_init();
  now_init();
 // web_init();
}

void loop() {
  //ws_loop();
  driver_loop();
}