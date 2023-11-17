#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <ArduinoJson.h>

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create a WebSocket object
AsyncWebSocket ws("/ws");

// Json Variable to Hold Sensor Readings
const uint8_t size = JSON_OBJECT_SIZE(3);
StaticJsonDocument<size> readings;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

// Get Sensor Readings and return JSON object
void getSensorReadings(String& json) {
  readings["temperature"] = String(12);
  readings["humidity"] =  String(12);
  readings["pressure"] = String(23/100.0F);
  // String jsonString = JSON.stringify(readings);
  serializeJson(readings, json);
}

void notifyClients(String sensorReadings) {
  ws.textAll(sensorReadings);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    //data[len] = 0;
    //String message = (char*)data;
    // Check if the message is "getReadings"
    //if (strcmp((char*)data, "getReadings") == 0) {
      //if it is, send current sensor readings
      String readings;
      getSensorReadings(readings);
      Serial.print(readings);
      notifyClients(readings);
    //}
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void ws_init() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void web_init() {
    // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.serveStatic("/", SPIFFS, "/");

  // Start server
  server.begin();
}

void SPIFFS_init() {
  if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}

void ws_loop() {
  if ((millis() - lastTime) > timerDelay) {
    String sensorReadings;
    getSensorReadings(sensorReadings);
    Serial.print(sensorReadings);
    notifyClients(sensorReadings);

  lastTime = millis();
  }

  ws.cleanupClients();
}