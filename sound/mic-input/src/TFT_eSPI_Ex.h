#pragma once

#include <TFT_eSPI.h>

uint32_t tft_color24to16(uint32_t color888)
{
  uint16_t r = (color888 >> 8) & 0xF800;
  uint16_t g = (color888 >> 5) & 0x07E0;
  uint16_t b = (color888 >> 3) & 0x001F;

  return (r | g | b);
}

#define TFT_DARK_RED tft_color24to16(0xDD0000)
#define TFT_DARK_DARK_GRAY tft_color24to16(0x141414)

typedef struct {
    int top;
    int left;
    int width;
    int height;
} TFT_Rect;

TFT_Rect resize(const TFT_Rect& rect, int left, int top, int right, int bottom) {
    return { 
        .top = rect.top + top, 
        .left = rect.left + left, 
        .width = rect.width - left - right, 
        .height = rect.height - top - bottom
    };
}

void fillRect(TFT_eSprite& sprite, const TFT_Rect& rect, int color) {
    sprite.fillRect(rect.left, rect.top, rect.width, rect.height, color);
}

void fillRectChevron(TFT_eSprite& sprite, int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color, uint32_t bg_color) {
    sprite.fillRect(x, y, w, h, bg_color);
    sprite.drawWideLine(x, y + h, x + w, y, w, color, bg_color);
}