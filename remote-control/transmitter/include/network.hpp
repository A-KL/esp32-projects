#ifndef network_h
#define network_h

#include <WiFi.h>

const char* ssid = "";
const char* password = "";

void wifi_init(const char * hostname = NULL) 
{
  WiFi.setHostname(hostname);
  
  WiFi.mode(WIFI_MODE_STA);
  Serial.println(WiFi.macAddress());
}

#endif