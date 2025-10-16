#pragma once 

class TFT_eSprite_GFX : public LGFX_Sprite
{
  public:
    // TFT_eSprite_GFX(LGFX* lcd) : LGFX_Sprite(lcd)
    // {}

    TFT_eSprite_GFX() : LGFX_Sprite() { _psram = true; }
    TFT_eSprite_GFX(LovyanGFX* parent) : LGFX_Sprite(parent) { _psram = true; }

    inline void pushToSprite(TFT_eSprite_GFX* sprite, int32_t x, int32_t y, int color)
    {
        this->pushSprite(sprite, x, y, color);
    }

    inline void pushToSprite(TFT_eSprite_GFX* sprite, int32_t x, int32_t y)
    {
        this->pushSprite(sprite, x, y);
    }

    inline void fillRectVGradient(int32_t x, int32_t y, uint32_t w, uint32_t h, const int start, const int end)
    {
        this->fillGradientRect(x, y, w, h, start, end, lgfx::VLINEAR);
    }

    inline void fillRectHGradient(int32_t x, int32_t y, uint32_t w, uint32_t h, const uint16_t start, const uint16_t end)
    {
        this->fillGradientRect(x, y, w, h, start, end, lgfx::HLINEAR);
    }
};