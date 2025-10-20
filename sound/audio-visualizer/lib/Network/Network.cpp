#include <WiFi.h>

#include "Network.h"

void setupWiFi()
{
  WiFi.disconnect();
  WiFi.softAPdisconnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int i = 0;

  Serial.print("WiFi [Connecting \'");
  Serial.print(WIFI_SSID);
  Serial.println("\']");

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);

    i = i + 1;

    if (i > 10) {
        Serial.println("WiFi [Failed. Reboot in 1s]");
        delay(1000);
        ESP.restart();
    }
  }

  Serial.println("WiFi [OK]");
  Serial.println(WiFi.localIP());
}