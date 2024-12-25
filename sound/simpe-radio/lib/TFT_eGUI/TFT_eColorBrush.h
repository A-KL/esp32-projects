#pragma once

#include <SPI.h>
#include <TFT_eSPI.h>

#include "TFT_eSPI_Ex.h"

class TFT_eColorBrush
{
    public:
        virtual void fillRect(TFT_eSprite* sprite, int32_t x, int32_t y, int32_t w, int32_t h) const = 0;

        virtual void fillRect(TFT_eSprite& sprite, int32_t x, int32_t y, int32_t w, int32_t h) const
        {
            fillRect(&sprite, x, y, w, h);
        }
};

class TFT_eSolidBrush : public TFT_eColorBrush
{
    public:
        TFT_eSolidBrush(uint32_t color) 
            : _color(color)
            { }

        TFT_eSolidBrush(uint32_t color, uint8_t luminance) 
            : _color(tft_luminance(color, luminance))
            { }

        void fillRect(TFT_eSprite* sprite, int32_t x, int32_t y, int32_t w, int32_t h) const
        {
            sprite->fillRect(x, y, w, h, _color);
        }

    private:
        uint32_t _color;
};

class TFT_eChevronBrush : public TFT_eColorBrush
{
    public:
        TFT_eChevronBrush(uint32_t chevron_color, uint32_t background_color) 
            : _chevron_color(chevron_color), _background_color(background_color)
            { }

        void fillRect(TFT_eSprite* sprite, int32_t x, int32_t y, int32_t w, int32_t h) const
        {
            sprite->fillRect(x, y, w, h, _background_color);

            auto ch_w = chevron_width > 0 ? chevron_width : h;
            auto left_x = -ch_w/2 + left; 

            while (left_x < w)
            {
                sprite->fillTriangle(
                    x + left_x, y, 
                    x + ch_w + left_x, y,
                    x + ch_w + left_x, y + ch_w,
                    _chevron_color);

                left_x += ch_w;

                sprite->fillTriangle(
                    x + left_x, y,
                    x + left_x + ch_w, y + ch_w,
                    x + left_x, y + ch_w, 
                    _chevron_color);

                left_x += ch_w;
            }
        }

        int16_t left = 0;
        int16_t chevron_width = -1;

    private:
        uint32_t _chevron_color;
        uint32_t _background_color;
};

class TFT_eGradientBrush : public TFT_eColorBrush
{
    public:
        TFT_eGradientBrush(uint32_t color1, uint32_t color2, bool fast = true) 
            : _color1(color1), _color2(color2), _fast(fast)
            { }

        TFT_eGradientBrush(uint32_t color, bool fast = true) 
            : _color1(color), _color2(tft_luminance(color, 100)), _fast(fast)
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