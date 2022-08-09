#include "sbus.h"
#include <M5Stack.h>
#include <Wire.h>
#include <esp_log.h>

#include "lego_plus_driver.h"

#define GFXFF 1
#define FF18  &FreeSans12pt7b
#define CF_OL24 &Orbitron_Light_24

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
      spr.setCursor(0, i * 20);
      spr.printf("CH%d: %d\n", i, perc(sbus_data[i]));

      Serial.print(sbus_data[i]);
      Serial.print("\t");
    }

    /* Display lost frames and failsafe data */
    Serial.print(sbus_rx.lost_frame());
    Serial.print("\t");
    Serial.println(sbus_rx.failsafe());
  }

  spr.setTextColor(GREEN); 

  for (int8_t i = 0; i < 4; i++) {
    spr.setCursor(150, i * 20 ); //+ 8 * 15
    spr.printf("Enc%d: %d\n", i, ReadEncoder(i));
  }

  spr.pushSprite(0, 0);
}