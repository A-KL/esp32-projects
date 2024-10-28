#pragma once

#include <WiFi.h>

//#include <sbus.h>
#include <esp_now.h>
#include <esp32-hal-log.h>

#include <driver_config.h>
#include <inputs_queue.h>

#define INPUT_ESP_NOW_MIN      0
#define INPUT_ESP_NOW_MAX      4095
#define INPUT_ESP_NOW_CHANNELS 16

struct enow_message_t {
  unsigned short  channels[INPUT_ESP_NOW_CHANNELS];
};
static enow_message_t message;

static queue_t<enow_message_t> enow_input_queue;



// unsigned long lastTime = 0;
// unsigned long elapsedTime = 0;
// unsigned long receiveDelay = 500;

// unsigned long receives_count = 0;
// unsigned long receives_count_max = 64;

inline uint8_t mac_to_string(const uint8_t * mac, char* result)
{
  const auto string_size = 18;

  snprintf(
    result, 
    string_size, 
    "%02x:%02x:%02x:%02x:%02x:%02x",
    mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    return string_size;
}

// useful for senders if receiver uses wifi network, at the same model
// int32_t getWiFiChannel(const char *ssid) {
//   if (int32_t n = WiFi.scanNetworks()) {
//       for (uint8_t i=0; i<n; i++) {
//           if (!strcmp(ssid, WiFi.SSID(i).c_str())) {
//               return WiFi.channel(i);
//           }
//       }
//   }
//   return 0;
// }

void enow_on_received(const uint8_t * mac, const uint8_t *data, int len) 
{
  memcpy(&message, data, sizeof(message));

#ifdef INPUT_ESP_NOW_DEBUG
  log_d("CPU Core: (%d) Values: %d %d %d %d %d %d %d %d %d %d ",
    xPortGetCoreID(),

    message.channels[0].value,
    message.channels[1].value,
    message.channels[2].value,
    message.channels[3].value,

    message.channels[4].value,
    message.channels[5].value,
    message.channels[6].value,
    message.channels[7].value,
    
    message.channels[8].value,
    message.channels[9].value);
#endif

  queue_send(enow_input_queue, message);

    // for (auto i=0; i<sbus_channels_count; i++) {
    //     sbus_data.ch[i] = map(message.channels[i].value, INPUT_ESP_NOW_MIN, INPUT_ESP_NOW_MAX, INPUT_SBUS_MIN, INPUT_SBUS_MAX);     
    // }

    // for (auto i=0; i<pwm_channels_count; i++) {
    //     //sbus_data.ch[i] = map(message.channels[i].value, INPUT_ESP_NOW_MIN, INPUT_ESP_NOW_MAX, INPUT_SBUS_MIN, INPUT_SBUS_MAX);   
    //     pwm_write(i, message.channels[i].value);
    // }
    
    // if (receives_count == 0)
    // {
    //   lastTime = millis();
    //   elapsedTime = 0;
    // } 
    // else if (receives_count >= receives_count_max)
    // {
    //   log_d("Latency %dms", (elapsedTime / receives_count));
    //   receives_count = 0;
    //   elapsedTime = 0;
    //   lastTime = millis();
    // }

    // elapsedTime += (millis() - lastTime);
    // receives_count++;
}

void enow_init() 
{
  // auto mode = WiFi.getMode();
  // if (mode != WIFI_MODE_STA && mode != WIFI_MODE_NULL) {
  //   WiFi.setSleep(false);
  //   log_i("Looks like WiFi is being used, setting sleep to false.");
  // } else {
  //   WiFi.mode(WIFI_MODE_STA);
  // }

  if (esp_now_init() != ESP_OK) 
  {
    log_e("ESP-NOW Initializing...FAILED");
    return;
  }

  log_i("ESP-NOW Initializing...OK");

  queue_init(enow_input_queue);

  esp_now_register_recv_cb(esp_now_recv_cb_t(enow_on_received));
}

uint8_t enow_receive(int16_t* outputs)
{
    static enow_message_t data;

    if (queue_receive(enow_input_queue, data))
    {
          memcpy(outputs, data.channels, sizeof(data.channels));

          return sizeof(data.channels) / sizeof(unsigned short);
    }

    return 0;
}