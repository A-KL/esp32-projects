#include <types.h>
#include <sbus.h>
#include <esp_now.h>
#include <esp32-hal-log.h>

#include <config_esp32_c3.h>
#include <pwm.h>

bfs::SbusTx sbus_tx(&Serial1, sbus_rx_tx_pins[0], sbus_rx_tx_pins[1], true);
bfs::SbusData sbus_data;

unsigned long lastTime = 0;
unsigned long receiveDelay = 500;

void OnEspNowReceived(const uint8_t * mac, const uint8_t *data, int len) 
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

    for (auto i=0; i<sbus_channels_count; i++) {
        sbus_data.ch[i] = map(message.channels[i].value, INPUT_ESP_NOW_MIN, INPUT_ESP_NOW_MAX, INPUT_SBUS_MIN, INPUT_SBUS_MAX); 
    }

    sbus_tx.data(sbus_data);
    sbus_tx.Write();

    lastTime = millis();
}

void now_init() {

  if (esp_now_init() != ESP_OK) {
    log_e("Error initializing ESP-NOW");
    return;
  }
  log_i("ESP-NOW initialized");
  
  esp_now_register_recv_cb(OnEspNowReceived);

  sbus_tx.Begin();

  //pwm_init();
  //pwm_start();
}

void now_loop() {
  if ((millis() - lastTime) > receiveDelay) {
    log_w("No data was received for %dms", receiveDelay);
   // pwm_stop();
   // delay(1000);
  }
}