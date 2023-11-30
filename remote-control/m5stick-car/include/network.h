#pragma once

#include <WiFi.h>
#include <esp32-hal-log.h>

const char* ssid = "";
const char* password = "";

void wifi_init(const char * hostname = NULL) 
{
  WiFi.setHostname(hostname);
  
  WiFi.mode(WIFI_MODE_STA);
  log_i("Client address: %s", WiFi.macAddress().c_str());

  // WiFi.begin(ssid, password);

  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  // }
}