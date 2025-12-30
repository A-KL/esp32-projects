#pragma once

struct TFT_Rect
{
    int top;
    int left;
    int width;
    int height;
};

uint16_t tft_color24to16(const uint32_t color888)
{
  uint16_t r = (color888 >> 8) & 0xF800;
  uint16_t g = (color888 >> 5) & 0x07E0;
  uint16_t b = (color888 >> 3) & 0x001F;

  return (r | g | b);
}

uint16_t tft_luminance(const uint16_t color, const uint8_t luminance)
{
    // Extract rgb colours and stretch range to 0 - 255
  uint16_t r = (color & 0xF800) >> 8; r |= (r >> 5);
  uint16_t g = (color & 0x07E0) >> 3; g |= (g >> 6);
  uint16_t b = (color & 0x001F) << 3; b |= (b >> 5);

  b = ((b * (uint16_t)luminance + 255) >> 8) & 0x00F8;
  g = ((g * (uint16_t)luminance + 255) >> 8) & 0x00FC;
  r = ((r * (uint16_t)luminance + 255) >> 8) & 0x00F8;

  return (r << 8) | (g << 3) | (b >> 3);
}

inline int border_with_padding(const int border, const int padding) 
{
    return border + (border > 0 ? padding : 0);
}

#define TFT_DARK_RED_8 tft_color24to16(0x880000)
#define TFT_DARK_RED_12 tft_color24to16(0xCC0000)
#define TFT_DARK_RED_13 tft_color24to16(0xDD0000)
#define TFT_DARK_DARK_GRAY tft_color24to16(0x252525)
#define TFT_DARK_GRAY tft_color24to16(0x404040)

TFT_Rect resize(const TFT_Rect& rect, const int left, const int top, const int right, const int bottom) {
    return { 
        .top = rect.top + top, 
        .left = rect.left + left, 
        .width = rect.width - left - right, 
        .height = rect.height - top - bottom
    };
}

inline void not_null(const void* object) {
    if (object == NULL) {
        log_e("Canvas can't b e NULL");
    }
}

void fillRect(TFT_eSprite& sprite, const TFT_Rect& rect, const uint16_t color) {
    sprite.fillRect(rect.left, rect.top, rect.width, rect.height, color);
}

void fillRectChevron(TFT_eSprite& sprite, int32_t x, int32_t y, int32_t w, int32_t h, uint16_t color, uint16_t bg_color) {
    sprite.fillRect(x, y, w, h, bg_color);
    sprite.drawWideLine(x, y + h, x + w, y, w, color);
}

float fmap(const float x, const float in_min, const float in_max, const float out_min, const float out_max) {
    const float run = in_max - in_min;
    if(run == 0){
        log_e("map(): Invalid input range, min == max");
        return -1; // AVR returns -1, SAM returns 0
    }
    const float rise = out_max - out_min;
    const float delta = x - in_min;
    return (delta * rise) / run + out_min;
}