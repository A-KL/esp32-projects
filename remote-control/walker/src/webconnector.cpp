// #include <Arduino.h>
#include <WiFi.h>

#include "webconnector.h"

const char* ssid = "Qwerty";
const char* password = "12345678";
WiFiServer server(80);

void WebConnector::init() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  server.begin();
  activeCommand = 5;
}

void WebConnector::handleConnection() {
  WiFiClient client = server.available();
  while (client.connected()) {
    if (client.available()) {
      while(client.available()) {
        input = client.readStringUntil('\n');
        if (input.startsWith("GET")) {
          // this is our get request, check if we have a parameter
          int commandPosition = input.indexOf("?command=");
          if (commandPosition > -1) {
            // a command, how exciting.
            activeCommand = input.substring(commandPosition+9, commandPosition+11);
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            client.println("<!DOCTYPE HTML><html></html>");
            client.stop();
          } else {
            // give back the ajax page with commands
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            client.println("<!DOCTYPE HTML><html></html>");
            client.stop();
          }
        }
      }
    }
  }
}

String WebConnector::getActiveCommand() {
  return activeCommand;
}
