#pragma once

#include <WiFi.h>
#include <esp32-hal-log.h>

const char* ssid = "";
const char* password = "";

void wifi_init_access_point(const char * hostname = NULL) 
{
  log_i("Setting AP (Access Point)…");

  WiFi.setHostname(hostname);
  WiFi.mode(WIFI_MODE_AP);
  WiFi.softAP(ssid, password);

  auto ip = WiFi.softAPIP();
  auto mac = WiFi.macAddress();

  log_i("IP: %s MAC: %s", ip, mac.c_str());
}

void wifi_init_client(const char * hostname = NULL) 
{
  WiFi.setHostname(hostname);
  
  WiFi.mode(WIFI_MODE_STA);
  log_i("MAC: %s", WiFi.macAddress().c_str());

  // WiFi.begin(ssid, password);

  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  // }
}