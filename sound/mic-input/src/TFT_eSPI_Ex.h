#pragma once

#include <TFT_eSPI.h>

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