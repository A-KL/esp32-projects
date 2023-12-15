#pragma once

#include "widgets.h"

//#define GFXFF 1
//#define FF18  &FreeSans12pt7b
//#define CF_OL24 &FreeMonoBold9pt7b
#define AA_FONT_SMALL NotoSansBold15

// GUI
const int margin = 5;

WidgetPanel sbus_panel(margin, margin * 1, WidgetPanel::Large, "sbus", COLOR_DARK_GRAY);
WidgetPanel ps3_panel(margin, margin * 2 + sbus_panel.Height, WidgetPanel::Small, "ps4", TFT_DARKGREEN, TFT_BLUE);

WidgetPanel nrf42_panel(margin * 2 + sbus_panel.Width, margin, WidgetPanel::Small, "nrf42", COLOR_DARK_RED, COLOR_RED);
WidgetPanel encoders_panel(margin * 2 + sbus_panel.Width, margin * 2 + nrf42_panel.Height, WidgetPanel::Medium, "encoders", COLOR_DARK_MAGENTA, COLOR_MAGENTA);
WidgetPanel motors_panel(margin * 2 + sbus_panel.Width, margin * 3 + encoders_panel.Height + nrf42_panel.Height, WidgetPanel::Small, "motors", COLOR_DARK_GREEN, COLOR_GREEN);

WidgetPanel power_panel(margin * 3 + sbus_panel.Width * 2, margin, WidgetPanel::Medium, "power", COLOR_DARK_YELLOW, COLOR_YELLOW);
WidgetPanel esp_now_panel(margin * 3 + sbus_panel.Width * 2, margin * 2 + + power_panel.Height, WidgetPanel::Large, "esp now", COLOR_DARK_GRAY);

WidgetList<8> sbus_values(sbus_panel, 0, widget_title_height, COLOR_DARK_GRAY);
WidgetList<2> ps3_values(ps3_panel, 0, widget_title_height, TFT_DARKGREEN);

WidgetList<2> nrf42_values(nrf42_panel, 0, widget_title_height, COLOR_DARK_RED);
WidgetList<4> encoder_values(encoders_panel, 0, widget_title_height, COLOR_DARK_MAGENTA);
WidgetList<2> motors_values(motors_panel, 0, widget_title_height, COLOR_DARK_GREEN);

WidgetList<3> power_values(power_panel, 0, widget_title_height, COLOR_DARK_YELLOW);

WidgetList<6> esp_now_values(esp_now_panel, 0, widget_title_height, COLOR_DARK_GRAY);

//WidgetListPanel<3> power(margin * 3 + sbus_panel.Width * 2, margin, WidgetPanel::Large, "power", COLOR_DARK_YELLOW, COLOR_YELLOW);

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);

void gui_init() {
  tft.init();
  tft.setRotation(TFT_ROTATE);
  tft.fillScreen(TFT_BLACK);

  // spr.setTextSize(2);
  // spr.setFreeFont(CF_OL24);
  // spr.setColorDepth(8);
  spr.setColorDepth(16); // 16 bit colour needed to show antialiased fonts
  spr.loadFont(AA_FONT_SMALL);

  sbus_panel.render(spr);
  ps3_panel.render(spr);
  nrf42_panel.render(spr);
  encoders_panel.render(spr);
  motors_panel.render(spr);
  power_panel.render(spr);
  power_panel.render(spr);
}