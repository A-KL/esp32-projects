#ifndef network_h
#define network_h

#include <WiFi.h>

const char* ssid = "";
const char* password = "";

void wifi_init() 
{
  WiFi.setHostname("esp-now-receiver-v1");
  
  WiFi.mode(WIFI_MODE_STA);
  
  Serial.println(WiFi.macAddress());

  // WiFi.begin(ssid, password);

  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  // }
}

#endif