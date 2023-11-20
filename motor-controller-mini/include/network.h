#ifndef network_h
#define network_h

#include <WiFi.h>

const char* ssid = "";
const char* password = "";

void wifi_init() 
{
  WiFi.setHostname("esp32-motor-ctrl-mini");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

#endif