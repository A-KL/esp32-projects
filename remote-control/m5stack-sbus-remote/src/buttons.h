#pragma once

#include <M5Unified.h>

void hal_buttons_click(int8_t i);

void hal_buttons_init()
{
  auto cfg = M5.config();
  M5.begin(cfg);
}

void hal_buttons_loop()
{
  M5.update();

  if (M5.BtnA.wasPressed()) {
      Serial.println("BtnA Pressed");
      hal_buttons_click(0);
  } else if (M5.BtnB.wasPressed()) {
      Serial.println("BtnB Pressed");
      hal_buttons_click(1);
  } else if (M5.BtnC.wasPressed()) {
      Serial.println("BtnC Pressed");
      hal_buttons_click(2);
  }
}