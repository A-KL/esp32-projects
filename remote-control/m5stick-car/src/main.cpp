#include <M5StickCPlus.h>
#include "CBACK_DRIVER.h"
#include <network.h>
#include <esp32_now.h>

CBACK_DRIVER driver;

void lcd_init() {
  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.setRotation(1);
  //M5.Lcd.drawCentreString("C BACK Driver", 120, 30, 4);
  M5.Lcd.setTextFont(2);
}

void driver_init() {
  driver.Init();
  driver.SetServoAngle(1, 90);
  driver.SetServoAngle(2, 90);
  driver.SetServoAngle(3, 90);
  driver.SetServoAngle(4, 90);
}

void setup() {
  Serial.begin(115200);

  M5.begin();

  driver_init();
  lcd_init();

  auto x = M5.IMU.Init();

  wifi_init("m5stick-car");
  now_init();
}

float pitch, roll, yaw;
float temp;

void loop() {
  M5.IMU.getTempData(&temp);
  M5.Lcd.setCursor(5, 5);
  M5.Lcd.printf("Temperature: %.2fC", (temp - 32) * 5.0/9);

  M5.IMU.getAhrsData(&pitch, &roll, &yaw);
  M5.Lcd.setCursor(5, 45);
  M5.Lcd.printf("X:%5.2f/nY:%5.2f/nZ:%5.2f ", pitch, roll, yaw);

  M5.Lcd.fillScreen(TFT_BLACK);

  now_loop();
  //M5.update();
}

void on_esp_now_disconnected() {
  driver.SetServoAngle(1, 90);
  driver.SetServoAngle(2, 90);
  driver.SetServoAngle(3, 90);
  driver.SetServoAngle(4, 90);
}

void on_esp_now_message_received(const data_message_t& data)
{
  auto left = map(data.channels[0].value, INPUT_ESP_NOW_MIN, INPUT_ESP_NOW_MAX, 0, 180);
  auto right = map(data.channels[1].value, INPUT_ESP_NOW_MIN, INPUT_ESP_NOW_MAX, 0, 180);

  driver.SetServoAngle(1, left);
  driver.SetServoAngle(2, right);

  log_w("Commands: %d\t %d", left, right);
}