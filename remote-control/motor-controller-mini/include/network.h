#pragma once

#include <WiFi.h>
#include <esp32-hal-log.h>

inline void init_wifi()
{
    uint8_t status = WiFi.status();
    uint8_t mode = WiFi.status();

    log_i("WiFi Status:%d Mode:%d", status, mode);

  // auto mode = WiFi.getMode();
  // if (mode != WIFI_MODE_STA && mode != WIFI_MODE_NULL) {
  //   WiFi.setSleep(false);
  //   log_i("Looks like WiFi is being used, setting sleep to false.");
  // } else {
  //   WiFi.mode(WIFI_MODE_STA);
  // }

    if (WiFi.status() == WL_CONNECTED) {
        //WiFi.setSleep(false);
        log_i("WiFi Station IP:%s Channel:%d MAC:%s", WiFi.localIP(), WiFi.channel(), WiFi.macAddress());
    } else {
        WiFi.mode(WIFI_AP_STA);
    }
}