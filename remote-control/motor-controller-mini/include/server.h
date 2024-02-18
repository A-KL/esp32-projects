#pragma once

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

#include "FS.h"
#include <LittleFS.h>

#include <types.h>
#include <config_esp32.h>

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create a WebSocket object
AsyncWebSocket ws("/ws");

// Json Variable to Hold Sensor Readings
// const uint8_t size = JSON_OBJECT_SIZE(3);
// StaticJsonDocument<size> readings;

// Timer variables
// unsigned long lastTime = 0;
// unsigned long timerDelay = 30000;

void Serialize(const motor_config_t* configs, const int config_size, String& result)
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

// void OnConfiguration(String& data)
// {
//   Serialize(motors_config, motors_count, data);
// }

// void NotifyClients(const String& data) {
//   Serial.println(data);
//   ws.textAll(data);
// }

void send_sbus_data(const int16_t values[], const short size) 
{
  StaticJsonDocument<200> doc;

  auto json = doc.createNestedArray("sbus_data");

  for (auto i = 0; i<size; ++i) {  
    json.add(values[i]);
  }

  String output;
  serializeJson(doc, output);
  ws.textAll(output);
}

void on_web_socket_data(const char* data, const size_t len)
{
    String message = data;

    Serial.print("->");
    Serial.println(message);

    if (strcmp((char*)data, "config") == 0) 
    {
      String data;
      //OnConfiguration(data);   
      //NotifyClients(data);
    }
}

void OnWebSocketMessage(void *arg, uint8_t *data, size_t len) 
{
  AwsFrameInfo *info = (AwsFrameInfo*)arg;

  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) 
  {
    data[len] = 0;
    on_web_socket_data((const char*)data, len);
  }
}

void OnSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      OnWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void OnNotFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void web_init() 
{
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", "text/html");
  });

  server.on("/restart", HTTP_POST, [](AsyncWebServerRequest *request) {
    request->send(200);
    ESP.restart();
  });

  server.on("/api/config", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/default.json", "application/json");
  });

  server.on("/api/v2/config", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/default_v2.json", "application/json");
  });

  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

  ws.onEvent(OnSocketEvent);

  server.addHandler(&ws);

  server.onNotFound(OnNotFound);

  server.begin();

  log_i("Web server is running on port 80");
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