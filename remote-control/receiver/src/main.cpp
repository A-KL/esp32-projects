#include <Arduino.h>

#include <network.h>
#include <storage.h>
#include <api.h>
#include <esp32_now.h>

void setup() {
  Serial.begin(115200);
  sleep(3);

  storage_init();
  wifi_init();
  api_init();
  now_init();

  //settings_load(motors_config, motors_count);

  //driver_init();
}

void loop() {
  //ws_loop();
  //driver_loop();
}