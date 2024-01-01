#pragma once

#include <SPI.h>
#include <TFT_eSPI.h>

#include "TFT_eSPI_Ex.h"

class TFT_eColorBrush
{
    public:
        virtual void fillRect(TFT_eSprite* sprite, int32_t x, int32_t y, int32_t w, int32_t h) const = 0;

        virtual void fillRect(TFT_eSprite& sprite, int32_t x, int32_t y, int32_t w, int32_t h)
        {
            fillRect(&sprite, x, y, w, h);
        }
};

class TFT_eSolidColorBrush : public TFT_eColorBrush
{
    public:
        TFT_eSolidColorBrush(uint32_t color) 
            : _color(color)
            { }

        void fillRect(TFT_eSprite* sprite, int32_t x, int32_t y, int32_t w, int32_t h) const
        {
            sprite->fillRect(x, y, w, h, _color);
        }

    private:
        uint32_t _color;
};

class TFT_eGradientColorBrush : public TFT_eColorBrush
{
    public:
        TFT_eGradientColorBrush(uint32_t color1, uint32_t color2, bool fast = true) 
            : _color1(color1), _color2(color2), _fast(fast)
            { }

        TFT_eGradientColorBrush(uint32_t color, bool fast = true) 
            : _color1(color), _color2(tft_luminance(color, 50)), _fast(fast)
            { }

        void fillRect(TFT_eSprite* sprite, int32_t x, int32_t y, int32_t w, int32_t h) const
        {
            if (_fast) {
                sprite->fillRect(x, y, w, h/2, _color1);
                sprite->fillRect(x, y + h/2, w, h/2, _color2);
            }
            else {
                sprite->fillRectVGradient(x, y, w, h, _color1, _color2);
            }
        }

    private:
        uint32_t _color1;
        uint32_t _color2;
        bool _fast;
};