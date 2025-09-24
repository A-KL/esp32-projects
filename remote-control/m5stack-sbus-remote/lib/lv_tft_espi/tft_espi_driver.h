#pragma once

#include <TFT_eSPI.h>

static TFT_eSPI tft = TFT_eSPI();

void lcd_init(uint32_t color = TFT_BLACK)
{
  tft.init();
  tft.setRotation(TFT_ROTATE);
  tft.fillScreen(color);
}

void lcd_display(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data)
{
    tft.startWrite();
    tft.setAddrWindow(x, y, w, h);
    tft.pushColors(data, w * h, true );
    tft.endWrite();
}