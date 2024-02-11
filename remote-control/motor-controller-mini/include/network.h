#pragma once

#include <WiFi.h>
#include <esp32-hal-log.h>

void wifi_init(const char* hostname = NULL, const char* ssid = NULL, const char* password = NULL) 
{
  WiFi.setHostname(hostname);
  WiFi.mode(WIFI_STA);

  log_i("MAC: %s", WiFi.macAddress().c_str());

  if (ssid == NULL || password == NULL) {
    return;
  }
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}