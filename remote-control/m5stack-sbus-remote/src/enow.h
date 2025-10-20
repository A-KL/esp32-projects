#include <WiFi.h>
#include <esp_now.h>
#include <esp32-hal-log.h>

#include <inputs_queue.h>

#define INPUT_ESP_NOW_MIN   0
#define INPUT_ESP_NOW_MAX   4000

#define ESP_NOW_CHANNELS_COUNT 10

struct channel_t {
    unsigned short value;
};

struct data_message_t {
    channel_t channels[ESP_NOW_CHANNELS_COUNT];
};

static queue_t<data_message_t> enow_input_queue;

unsigned long lastUpdated = 0;
unsigned long lastTime = 0;
unsigned long elapsedTime = 0;
unsigned long receiveDelay = 500;

unsigned long receives_count = 0;
unsigned long receives_count_max = 64;

// void on_esp_now_message_received(const data_message_t& data) {
//     for (uint8_t i = 0; i < ch_count; i++) {
//       esp_now_values.setValue(i, data.channels[i].value);
//     }
//   // auto grab = map(data.channels[0].value, INPUT_ESP_NOW_MIN, INPUT_ESP_NOW_MAX, 0, 180);
//   // auto left = map(data.channels[3].value, INPUT_ESP_NOW_MIN, INPUT_ESP_NOW_MAX, 180, 0);
//   // auto right = map(data.channels[5].value, INPUT_ESP_NOW_MIN, INPUT_ESP_NOW_MAX, 10, 180);

//   // driver.SetServoAngle(2, left);
//   // driver.SetServoAngle(1, right);
//   // driver.SetServoAngle(4, grab);

//   // log_w("Commands: %d\t %d", left, right);
// }
//void on_esp_now_disconnected();

// void on_esp_now_message_received(const data_message_t& data)
// {
// }

void on_esp_now_disconnected() 
{
    esp_now_values.clearValues();
}

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

  // log_d("%d %d %d %d %d %d %d %d %d %d ", 
  //   message.channels[0].value,
  //   message.channels[1].value,
  //   message.channels[2].value,
  //   message.channels[3].value,
  //   message.channels[4].value,
  //   message.channels[5].value,
  //   message.channels[6].value,
  //   message.channels[7].value,
  //   message.channels[8].value,
  //   message.channels[9].value);

    //on_esp_now_message_received(message);

    queue_send(enow_input_queue, message);
    
    measure_latency();
}

bool enow_init() 
{
  WiFi.mode(WIFI_MODE_STA);
  
  if (esp_now_init() != ESP_OK)
  {
    log_e("Error initializing ESP-NOW");
    return false;
  }

  lastUpdated = millis();

  queue_init(enow_input_queue, 10);
  esp_now_register_recv_cb(on_esp_now_data_received);

  log_i("ESP-NOW initialized"); 
  return true;
}

uint8_t enow_receive(int16_t* outputs)
{
    if (millis() - lastUpdated > receiveDelay)
    {
      on_esp_now_disconnected();
      return 0;
    }

    static data_message_t data;

    if (queue_receive(enow_input_queue, data))
    {
        for (uint8_t i = 0; i < esp_now_values.count(); i++) {
          esp_now_values.setValue(i, data.channels[i].value);
        }
        memcpy(outputs, data.channels, sizeof(data));
        return sizeof(data.channels) / sizeof(unsigned short);
    }

    return 0;
}

// void enow_loop() 
// {
//   if (millis() - lastUpdated > receiveDelay){
//     on_esp_now_disconnected();
//   }
// }