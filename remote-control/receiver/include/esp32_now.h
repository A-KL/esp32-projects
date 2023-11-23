#include <types.h>
#include <sbus.h>
#include <esp_now.h>
//#include <ESP32Servo.h>

#include <config_esp32.h>
#include <config_esp32_c3_v2.h>

bfs::SbusTx sbus_tx(&Serial1, sbus_rx_tx_pins[0], sbus_rx_tx_pins[1], true);
bfs::SbusData sbus_data;

void OnEspNowReceived(const uint8_t * mac, const uint8_t *incomingData, int len) 
{
  memcpy(&message, incomingData, sizeof(message));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Char: ");
  Serial.println(message.channels[0].value);
  Serial.print("Int: ");
  Serial.println(message.channels[1].value);
  Serial.print("Float: ");
  Serial.println(message.channels[2].value);
  Serial.print("Bool: ");
  Serial.println(message.channels[3].value);
  Serial.println();
}

void now_init()
{
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnEspNowReceived);

  sbus_tx.Begin();
}