#include <Arduino.h>

#include <network.h>
#include <storage.h>
#include <esp32_now.h>

#include <driver.h>

driver_strategy_t<int16_t> driver(-255, 255, 0);

void setup() {
  Serial.begin(115200);

  delay(5000);

  driver.write(255);

  // storage_init();
  // //settings_load(motors_config, motors_count);
  
  // wifi_init();
  // now_init();

  //api_init();
}

void loop() {
  auto speed = driver.read();
  log_d("Speed: %d (%d)", speed, millis());
  delay(50);
}