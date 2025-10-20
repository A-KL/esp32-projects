#pragma once

#include <list>
#include <WiFi.h>
#include <esp32-hal-log.h>
#include <esp_now.h>

#ifdef ESP_NOW_JSON_SETTINGS
  #include <ArduinoJson.h>
  #include <storage.hpp>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define ESP_NOW_CHANNELS_COUNT 20

const uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

struct channel_t {
unsigned short value;
};

struct data_message_t {
    channel_t channels[ESP_NOW_CHANNELS_COUNT];
};

data_message_t data_message;

esp_now_peer_info_t peer_info = {};

void init_peer_info(esp_now_peer_info& info, const String& mac, uint8_t channel = 0, bool encrypted = false)
{
    sscanf(mac.c_str(), "%2hhx:%2hhx:%2hhx:%2hhx:%2hhx:%2hhx", 
      &info.peer_addr[0], 
      &info.peer_addr[1], 
      &info.peer_addr[2], 
      &info.peer_addr[3], 
      &info.peer_addr[4], 
      &info.peer_addr[5]);

    info.channel = channel;  
    info.encrypt = encrypted;
}

void on_esp_now_data_sent(const uint8_t *mac_addr, esp_now_send_status_t status) 
{
  //log_d("Last Packet Send Status:%s", status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void now_init() {
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    log_e("Error initializing ESP-NOW");
    return;
  }

  log_i("ESP-NOW initialized");

  esp_now_register_send_cb(on_esp_now_data_sent);
}

void now_add_peer(const uint8_t* receiveAddress) {
  memcpy(peer_info.peer_addr, receiveAddress, 6);
  peer_info.channel = 0;  
  peer_info.encrypt = false;

  if (esp_now_add_peer(&peer_info) != ESP_OK) {
    log_e("Failed to pair client");
  }
}

void now_add_peer_str(const String address) {
  init_peer_info(peer_info, address);

  auto res = esp_now_add_peer(&peer_info);

  if (res == ESP_OK) {
    log_i("Client paired (%s)", address.c_str());
    return;
  }

  log_e("Failed to pair client (%s): %d", address.c_str(), res);
}

bool now_send(const data_message_t& data) {
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &data, sizeof(data_message_t));
  
  if(result != ESP_OK) {
    log_e("Failed data sending: %d", result);
  }

  return result == ESP_OK;
}

#ifdef ESP_NOW_JSON_SETTINGS

bool now_parse_peer(JsonVariant& element) {
  auto mac = element.as<String>();
  now_add_peer_str(mac);
  return true;
}

void now_add_peers_json(const char* filePath = "/default.json") {
  settings_load_array(now_parse_peer, "paired", filePath);
}

#endif

#ifdef __cplusplus
}
#endif