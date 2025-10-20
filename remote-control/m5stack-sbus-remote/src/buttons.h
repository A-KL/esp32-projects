#pragma once

#include <M5Unified.h>
//#include "OneButton.h"

// OneButton button_a(39);
// OneButton button_b(38);
// OneButton button_c(37);

void hal_buttons_click(int8_t i);

void hal_buttons_init()
{
  auto cfg = M5.config();
  cfg.clear_display = false;
  M5.begin(cfg);

  // button_a.attachClick([]() {
  //   hal_buttons_click(0);
  // });
  // button_b.attachClick([]() {
  //   hal_buttons_click(1);
  // });
  // button_c.attachClick([]() {
  //   hal_buttons_click(2);
  // });
}

void hal_buttons_loop()
{
  // button_a.tick();
  // button_b.tick();
  // button_c.tick();

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