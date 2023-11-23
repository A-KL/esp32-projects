#include <types.h>
#include <sbus.h>
#include <esp_now.h>
#include <ESP32Servo.h>

#include <config_esp32.h>
#include <config_esp32_c3_v2.h>

bfs::SbusTx sbus_tx(&Serial1, sbus_rx_tx_pins[0], sbus_rx_tx_pins[1], true);
bfs::SbusData sbus_data;

Servo servos[sizeof(pwm_pins)];

void pwm_init()
{
    ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);

    for (int i = 0; i < sizeof(pwm_pins); i++)
    {
        if (!ESP32PWM::hasPwm(pwm_pins[i])){
            log_w("Pin %d doesn't support PWM",i);
        }
        else
        {
            servos[i].setPeriodHertz(50);
        }
    }
}

void pwm_start() {
    for (int i = 0; i < sizeof(pwm_pins); i++) {
        if (!servos[i].attached()) {
            servos[i].attach(pwm_pins[i], 1000, 2000);
        }
    }
}

void pwm_stop() {
    for (int i = 0; i < sizeof(servos); i++) {
        if (servos[i].attached()) {
            servos[i].detach();
        }
    }
}

void pwm_write(const int values[], int size) {
    for (int i = 0; i < size; i++) {
        if (servos[i].attached()) {
            servos[i].write(values[i]);
        }
        else {
            log_w("PWM %d is detached",i);
        }
    }
}

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