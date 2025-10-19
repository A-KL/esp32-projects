#include <Arduino.h>

#include <network.h>
#include <storage.h>
#include <api.h>
#include <esp32_now.h>

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  sleep(3);

  storage_init();
  //settings_load(motors_config, motors_count);
  
  wifi_init();
  now_init();

  //api_init();
}

void loop() {
  now_loop();
}