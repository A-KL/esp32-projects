#include <Arduino.h>
//#include <DNSServer.h>

#include <network.h>
#include <driver_v2.h>
#include <storage.h>
#include <api.h>

//DNSServer dnsServer;

void setup() {
  Serial.begin(115200);
  sleep(3);

  storage_init();
  //wifi_init();
  api_init();

  settings_load(motors_config, motors_count);
 // ws_init();
 // web_init();

  driver_init();
}

void loop() {
  //ws_loop();
  driver_loop();
}