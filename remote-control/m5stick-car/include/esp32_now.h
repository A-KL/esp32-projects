#include <esp_now.h>
#include <esp32-hal-log.h>

#define INPUT_ESP_NOW_MIN   0
#define INPUT_ESP_NOW_MAX   4095

#define CHANNELS_COUNT 16

struct channel_t {
  unsigned short value;
};

struct data_message_t {
  channel_t channels[CHANNELS_COUNT];
};

data_message_t message;

unsigned long lastTime = 0;
unsigned long elapsedTime = 0;
unsigned long receiveDelay = 500;

unsigned long receives_count = 0;
unsigned long receives_count_max = 64;

void on_esp_now_message_received(const data_message_t& data);

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
}

void on_esp_now_data_received(const uint8_t * mac, const uint8_t *data, int len) 
{
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

    on_esp_now_data_received(message);
    
    measure_latency();
}

void now_init() {

  if (esp_now_init() != ESP_OK) {
    log_e("Error initializing ESP-NOW");
    return;
  }

  log_i("ESP-NOW initialized");
  
  esp_now_register_recv_cb(on_esp_now_data_received);
}