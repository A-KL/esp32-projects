#ifndef network_h
#define network_h

#include <WiFi.h>

const char* ssid = "";
const char* password = "";

void wifi_init() 
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi ..");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

#endif