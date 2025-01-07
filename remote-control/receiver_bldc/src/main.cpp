#include <Arduino.h>

#ifndef CONFIG_FILE
#error "Config file was not defined"
#endif
#include CONFIG_FILE

// #include <network.h>
// #include <storage.h>
// #include <esp32_now.h>

#include <driver.h>
#include <motor_output.h>
#include <ps_input.h>

driver_strategy_t<int16_t> driver(-255, 255, 0);

void setup() {
  Serial.begin(115200);

  delay(5000);

  ps_init();

  driver.write(-255);

  motors_init();

  // storage_init();
  // settings_load(motors_config, motors_count);
  
  // wifi_init();
  // now_init();

  // api_init();
}

void loop() {
  auto speed = driver.read();
  log_d("Speed: %d (%d)", speed, millis());
  delay(50);
}