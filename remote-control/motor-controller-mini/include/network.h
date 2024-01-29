#pragma once

#include <WiFi.h>

const char* ssid = "";
const char* password = "";

void wifi_init(const char* hostname = NULL) 
{
  WiFi.setHostname(hostname);
  WiFi.mode(WIFI_STA);

  Serial.println(WiFi.macAddress());

  // WiFi.begin(ssid, password);

  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  // }
}