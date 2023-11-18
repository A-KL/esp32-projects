#ifndef web_h
#define web_h

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <ArduinoJson.h>

#include <types.h>
#include <config_esp32.h>
#include <config_esp32_c3.h>

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

void serialize(const motor_config_t* configs, const int config_size, String& result)
{
  StaticJsonDocument<200> doc;

  for(auto i = 0; i<config_size; ++i)
  {
    JsonObject motor = doc.createNestedObject();

    motor["mode"] = configs[i].mode;
    motor["inverted"] = configs[i].inverted;
    motor["input_type"] = configs[i].input_type;
    motor["input_channel"] = configs[i].input_channel;
  }

  serializeJson(doc, result);
}

// void serialize(JsonDocument doc, const motor_config_t config, String& result)
// {
//   JsonObject motor = doc.createNestedObject();

//   motor["mode"] = config.mode;
//   motor["inverted"] = config.inverted;
//   motor["input_type"] = config.input_type;
//   motor["input_channel"] = config.input_channel;

//   serializeJson(doc, result);
// }

void on_configuration(String& data)
{
  serialize(motors, sizeof(motors), data);
}

void notifyClients(const String& data) {
  Serial.println(data);
  ws.textAll(data);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) 
{
  AwsFrameInfo *info = (AwsFrameInfo*)arg;

  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) 
  {
    data[len] = 0;
    String message = (char*)data;

    Serial.print("->");
    Serial.println(message);

    if (strcmp((char*)data, "config") == 0) 
    {
      String data;
      on_configuration(data);   
      notifyClients(data);
    }
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
  // if ((millis() - lastTime) > timerDelay) {
  //   String sensorReadings;
  //   getSensorReadings(sensorReadings);
  //   Serial.print(sensorReadings);
  //   notifyClients(sensorReadings);

  //   lastTime = millis();
  // }

  // ws.cleanupClients();
}

#endif