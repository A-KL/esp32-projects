#pragma once

#include <SPI.h>
#include <TFT_eSPI.h>

#include "TFT_eSPI_Ex.h"

class TFT_eWidget
{
    public:
        TFT_eWidget(TFT_eSprite& canvas, int16_t width, int16_t height, int16_t top = 0, int16_t left = 0) 
        : _canvas(&canvas), width(width), height(height), left(left), top(top)
        { }

        int16_t width;
        int16_t height;
        int16_t top;
        int16_t left;

        virtual void init() = 0;

        virtual void begin()
        {
            update();
        }

        virtual void update() = 0;

    private:
        TFT_eSprite* _canvas = NULL;
};

class TFT_eCassettePlayer : public TFT_eWidget
{
    public:
        // TFT_eCassettePlayer(TFT_eSprite& canvas, int16_t width, int16_t height int16_t width, int16_t height) 
        // : _canvas(&canvas), _width(width), _height(height)
        // { 

        // }

        void init()
        {   
            not_null(_canvas);

            _top_height = (_height - cassette_height) / 2;

            _canvas->setColorDepth(16);
            _canvas->setSwapBytes(true);
        }

        void update()
        {
            not_null(_canvas);

            // Background

            _canvas->createSprite(_width, cassette_height);
            _canvas->fillSprite(_color_background);
            _canvas->pushSprite(left, top + _top_height);
            _canvas->deleteSprite();

            // Cassette

            auto left_location = width / 3;
            auto right_location = left_location * 2;
            auto big_diameter = height / 2;
            auto small_diameter = height / 3;

            _canvas->createSprite(big_diameter, big_diameter);
            _canvas->drawSmoothCircle(0, 0, big_diameter/2, _color_empty, TFT_TRANSPARENT);
            _canvas->drawSmoothCircle(0, 0, small_diameter/2, _color_full, TFT_TRANSPARENT);
            _canvas->drawSmoothCircle(0, 0, small_diameter/3, _color_background, TFT_TRANSPARENT);
            _canvas->pushSprite(left, top);
            _canvas->deleteSprite();

            // Top / Bottom regions

            _canvas->createSprite(_width, _top_height);
            _canvas->fillSprite(_color_none);

            _canvas->pushSprite(left, top);
            _canvas->pushSprite(left, top + height - _top_height);
        }

        uint16_t cassette_height = _height * 0.4;

        int16_t _width;
        int16_t _height;

    private:
        uint16_t _top_height;

        uint32_t _color_none = TFT_BLACK;
        uint32_t _color_background = TFT_BLACK;
        uint32_t _color_full = TFT_DARK_GRAY;
        uint32_t _color_empty = TFT_LIGHTGREY;
        uint32_t _color_accent = TFT_RED;

        TFT_eSprite* _canvas = NULL;
};
