#include <WiFi.h>
#include <esp_now.h>
#include <esp32-hal-log.h>

#define INPUT_ESP_NOW_MIN   0
#define INPUT_ESP_NOW_MAX   4000

#define ESP_NOW_CHANNELS_COUNT 10

struct channel_t {
unsigned short value;
};

struct data_message_t {
    channel_t channels[ESP_NOW_CHANNELS_COUNT];
};

unsigned long lastUpdated = 0;
unsigned long lastTime = 0;
unsigned long elapsedTime = 0;
unsigned long receiveDelay = 500;

unsigned long receives_count = 0;
unsigned long receives_count_max = 64;

void on_esp_now_message_received(const data_message_t& data);

void on_esp_now_disconnected();

void measure_latency()
{
    if (receives_count == 0)
    {
      lastTime = millis();
      elapsedTime = 0;
    } 
    else if (receives_count >= receives_count_max)
    {
      log_d("Latency %dms", (elapsedTime / receives_count));
      receives_count = 0;
      elapsedTime = 0;
      lastTime = millis();
    }

    elapsedTime += (millis() - lastTime);
    receives_count++;

    lastUpdated = millis();
}

void on_esp_now_data_received(const uint8_t * mac, const uint8_t *data, int len) 
{
  data_message_t message;

  memcpy(&message, data, sizeof(message));

  log_d("%d %d %d %d %d %d %d %d %d %d ", 
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

    on_esp_now_message_received(message);
    
    measure_latency();
}

void now_init() {
  WiFi.mode(WIFI_MODE_STA);
  
  if (esp_now_init() != ESP_OK) {
    log_e("Error initializing ESP-NOW");
    return;
  }

  log_i("ESP-NOW initialized");
  
  esp_now_register_recv_cb(on_esp_now_data_received);

  lastUpdated = millis();
}

void now_loop() {
  if (millis() - lastUpdated > receiveDelay){
    on_esp_now_disconnected();
  }
}