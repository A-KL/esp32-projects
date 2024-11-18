#pragma once

#include <SPI.h>
#include <TFT_eSPI.h>

#include "TFT_eSPI_Ex.h"

class TFT_eWidget
{
    public:
        TFT_eWidget(TFT_eSPI* tft, int16_t width, int16_t height, int16_t top, int16_t left) 
        : _canvas(tft), _canvas_to_rotate(tft), width(width), height(height), left(left), top(top)
        { }

        int16_t width;
        int16_t height;
        int16_t top;
        int16_t left;

        virtual void init() = 0;

        virtual void begin() {
            update();
        }

        virtual void update() = 0;

        void load_font(const uint8_t array[])
        {
            _canvas.loadFont(array);
            _canvas_to_rotate.loadFont(array);
        }

    protected:
        TFT_eSprite _canvas;
        TFT_eSprite _canvas_to_rotate;
};