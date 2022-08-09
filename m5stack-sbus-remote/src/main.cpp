#include "sbus.h"
#include <M5Unified.h>

#include <esp_log.h>

bfs::SbusRx sbus_rx(&Serial1);
std::array<int16_t, bfs::SbusRx::NUM_CH()> sbus_data;

const int min_ch_value = 200;
const int max_ch_value = 1800;

const int max_ch = 8;

int16_t perc(int16_t value)
{
  return map(value, min_ch_value, max_ch_value, 0, 100);
}

void setup() {
  Serial.begin(115200);

  while (!Serial) {}

  sbus_rx.Begin(16, 17);
  M5.begin();

  M5.Display.setEpdMode(epd_mode_t::epd_fastest);
  M5.Display.startWrite();
  M5.Display.setTextSize(2);
}

void loop() {
  M5.update();

  if (sbus_rx.Read()) 
  {
    sbus_data = sbus_rx.ch();

    for (int8_t i = 0; i < max_ch; i++) {
      M5.Lcd.setCursor(5, i * 15);
      M5.Lcd.print("CH");
      M5.Lcd.print(i);
      M5.Lcd.print(": ");
      M5.Lcd.println(perc(sbus_data[i]));
      Serial.print(sbus_data[i]);
      Serial.print("\t");
    }

    /* Display lost frames and failsafe data */
    Serial.print(sbus_rx.lost_frame());
    Serial.print("\t");
    Serial.println(sbus_rx.failsafe());

    M5.Display.display();
  }
}