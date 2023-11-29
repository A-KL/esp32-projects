#include <M5StickCPlus.h>
#include "CBACK_DRIVER.h"
#include <network.h>
#include <esp32_now.h>

CBACK_DRIVER driver;

void init_lcd() {
    M5.Lcd.setTextColor(TFT_GREEN);
    M5.Lcd.setRotation(1);
    M5.Lcd.drawCentreString("C BACK Driver", 120, 30, 4);
    M5.Lcd.drawCentreString("Click Btn A Start", 120, 65, 4);
}

void setup() {
  Serial.begin(115200);

  M5.begin();
  driver.Init();
  auto x = M5.IMU.Init();

  wifi_init("m5stick-car");
  now_init();

  M5.Lcd.setTextColor(TFT_GREEN);
  M5.Lcd.setRotation(1);
  M5.Lcd.drawCentreString("C BACK Driver", 120, 30, 4);
  M5.Lcd.drawCentreString("Click Btn A Start", 120, 65, 4);

  driver.SetServoAngle(1, 90);
  driver.SetServoAngle(2, 90);
  driver.SetServoAngle(3, 90);
  driver.SetServoAngle(4, 90);
}

float pitch, roll, yaw;
float temp;

void loop() {
  // Driver.SetServoAngle(i, 120);
  M5.IMU.getAhrsData(&pitch, &roll, &yaw);
  M5.Lcd.setCursor(0, 45);
  M5.Lcd.printf("X:%5.2f/nY:%5.2f/nZ:%5.2f ", pitch, roll, yaw);

  M5.IMU.getTempData(&temp);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.printf("Temperature : %.2f C", temp);

  delay(500);
  //M5.update();
}

void on_esp_now_message_received(const data_message_t& data)
{
  log_w("got data");
}