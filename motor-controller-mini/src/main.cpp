#include <Arduino.h>
#include <DNSServer.h>
#include <WiFi.h>

//#include <driver.h>
#include <driver_v2.h>
#include <server.h>
#include <settings.h>

const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

DNSServer dnsServer;

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

void setup() {
  Serial.begin(115200);

  //wifi_init();
  //SPIFFS_init();

 // ws_init();
  //web_init();

  driver_init();
}

void loop() {
  //ws_loop();
  driver_loop();
}