#include "sbus.h"
#include <M5Stack.h>
#include <Wire.h>
#include <esp_log.h>
#include <Adafruit_INA219.h>
#include "lego_plus_driver.h"

#define GFXFF 1
#define FF18  &FreeSans12pt7b
#define CF_OL24 &FreeSans12pt7b

Adafruit_INA219 ina219_output(INA219_ADDRESS);
Adafruit_INA219 ina219_input(INA219_ADDRESS + 1);

bfs::SbusRx sbus_rx(&Serial1);
std::array<int16_t, bfs::SbusRx::NUM_CH()> sbus_data;

const int min_ch_value = 200;
const int max_ch_value = 1800;

const int max_ch = 8;

TFT_eSprite spr = TFT_eSprite(&M5.Lcd);

int16_t perc(int16_t value)
{
  return map(value, min_ch_value, max_ch_value, 0, 100);
}

void setup() {
  M5.begin();
  M5.Power.begin();

  Wire.begin();
  Serial.begin(115200);

  if (!ina219_output.begin()) {
    Serial.println("Failed to find output INA219 chip");
  }

  if (!ina219_input.begin()) {
    Serial.println("Failed to find input INA219 chip");
  }

  sbus_rx.Begin(16, 17);

  M5.Lcd.fillScreen(TFT_BLACK);

  //spr.setTextSize(2);
  spr.setFreeFont(CF_OL24);
  spr.setColorDepth(8);
  spr.createSprite(320, 240);
}

void loop() {
  spr.fillSprite(TFT_BLACK);

  if (sbus_rx.Read())
  {
    sbus_data = sbus_rx.ch();

    spr.setTextColor(WHITE); 

    for (int8_t i = 0; i < max_ch; i++) {
      spr.setCursor(0, (i + 1) * 20);
      spr.printf("CH%d: %d\n", i, perc(sbus_data[i]));

      Serial.print(sbus_data[i]);
      Serial.print("\t");
    }

    auto left_speed = map(sbus_data[2], min_ch_value, max_ch_value, -255, 255);
    auto right_speed = map(sbus_data[1], min_ch_value, max_ch_value, 255, -255);

    spr.setTextColor(RED); 
    spr.setCursor(0, (10 * 20));
    spr.printf("Left: %d\n", left_speed);
    spr.setCursor(0, (11 * 20));
    spr.printf("Right: %d\n", right_speed);

    MotorRun(1, left_speed);
    MotorRun(2, left_speed);

    MotorRun(3, right_speed);
    MotorRun(0, right_speed);

    /* Display lost frames and failsafe data */
    Serial.print(sbus_rx.lost_frame());
    Serial.print("\t");
    Serial.println(sbus_rx.failsafe());
  }

  spr.setTextColor(GREEN); 

  for (int8_t i = 0; i < 4; i++) {
    spr.setCursor(150, (i + 1) * 20 ); //+ 8 * 15
    spr.printf("Enc%d: %d\n", i, ReadEncoder(i));
  }

  auto shuntvoltage = ina219_output.getShuntVoltage_mV();
  auto busvoltage = ina219_output.getBusVoltage_V();
  auto current_mA = ina219_output.getCurrent_mA();
  auto power_mW = ina219_output.getPower_mW();
  auto loadvoltage = busvoltage + (shuntvoltage / 1000);

  spr.setTextColor(ORANGE); 

  spr.setCursor(150, 110);
  spr.printf("V: %.2fV\n", busvoltage);
  spr.setCursor(150, 130);
  spr.printf("I: %.2fmA\n", current_mA);

  spr.setCursor(150, 160);
  spr.printf("V: %.2fV\n", ina219_input.getBusVoltage_V());
  spr.setCursor(150, 180);
  spr.printf("I: %.2fmA\n", ina219_input.getCurrent_mA());

  spr.pushSprite(0, 0);
}