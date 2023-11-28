#pragma once

#include <WiFi.h>
#include <esp_now.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ESP_NOW_CHANNELS_COUNT 10

const uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const uint8_t receiveAddress[] = {0x84, 0xFC, 0xE6, 0x00, 0x27, 0x9C };

struct channel_t {
unsigned short value;
};

struct data_message_t {
    channel_t channels[ESP_NOW_CHANNELS_COUNT];
};

data_message_t data_message;

esp_now_peer_info_t peerInfo;

void EspNowOnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) 
{
  //log_d("Last Packet Send Status:%s", status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setupEspNow() {
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    log_e("Error initializing ESP-NOW");
    return;
  }

  log_i("ESP-NOW initialized");

  esp_now_register_send_cb(EspNowOnDataSent);

  memcpy(peerInfo.peer_addr, receiveAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    log_e("Failed to add peer");
    return;
  }
}

void sendEspNow(const data_message_t& data)
{
  esp_err_t result = esp_now_send(receiveAddress, (uint8_t *) &data, sizeof(data_message_t));
   
  if (result != ESP_OK) {
    log_e("Error sending the data");
  }
}

#ifdef __cplusplus
}
#endif