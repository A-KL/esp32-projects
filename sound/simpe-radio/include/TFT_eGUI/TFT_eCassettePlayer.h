#pragma once

#include <SPI.h>
#include <TFT_eSPI.h>

#include "TFT_eSPI_Ex.h"

class TFT_eWidget
{
    public:
        TFT_eWidget(TFT_eSprite* canvas, int16_t width, int16_t height, int16_t top, int16_t left) 
        : _canvas(canvas), width(width), height(height), left(left), top(top)
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

    protected:
        TFT_eSprite* _canvas = NULL;
};

class TFT_eCassettePlayer : public TFT_eWidget
{
    public:
        TFT_eCassettePlayer(TFT_eSprite* canvas, int16_t width, int16_t height, int16_t top = 0, int16_t left = 0) 
        : TFT_eWidget(canvas, width, height, top, left)
        { }

        void init()
        {   
            not_null(_canvas);

            _top_height = (height - cassette_height) / 2;
        }

        void update()
        {
            update(3);
        }

        void update(int16_t update_index = 3)
        {
            not_null(_canvas);

            // Background
            if (update_index >= 3)
            {
                _canvas->createSprite(width, cassette_height);
                _canvas->setColorDepth(16);
                _canvas->setSwapBytes(true);
                _canvas->fillSprite(_color_cassette_bg);
                _canvas->pushSprite(left, top + _top_height);
                _canvas->deleteSprite();
            }

            auto outer_diameter = height;
            auto tape_diameter = outer_diameter * 0.8;

            auto left_location = width / 8;
            auto right_location = left_location * 2;
            auto radius = outer_diameter / 2;

            // Cassette
            if (update_index >= 2)
            {
                _canvas->createSprite(outer_diameter, outer_diameter);
                _canvas->setColorDepth(16);
                _canvas->setSwapBytes(true);
                _canvas->fillSprite(TFT_TRANSPARENT);

                _canvas->fillSmoothCircle(radius, radius, radius, _color_cassette);
                _canvas->fillSmoothCircle(radius, radius, tape_diameter/2.5, _color_tape);
                _canvas->fillSmoothCircle(radius, radius, tape_diameter/3, TFT_BLACK);
                _canvas->fillSmoothCircle(radius, radius, tape_diameter/3.2, _color_accent);

                //_canvas->fillSmoothCircle(radius, radius, tape_diameter/5, TFT_BLACK);

                _canvas->pushSprite(left + left_location - radius, top, TFT_TRANSPARENT);
                _canvas->pushSprite(left + left_location * 7 - radius, top, TFT_TRANSPARENT);

                _canvas->deleteSprite();
            }

           // if (update_index >= 1)
            {
                auto sprite_w = tape_diameter/3.2;
                auto sprite_h = sprite_w;
                auto h = sprite_w / 10;
                auto w = sprite_w / 5;

                _canvas->createSprite(100, 100);
                _canvas->setColorDepth(16);
                _canvas->setSwapBytes(true);
                _canvas->fillSprite(TFT_TRANSPARENT);
                //_canvas->setPivot(radius, radius);

                _canvas->fillSmoothCircle(10, 10, 20, TFT_BLACK);

                //_canvas->fillRect(0, sprite_h/2 - h/2, w, h, _color_accent);
               // _canvas->fillRect(sprite_w-w, sprite_h/2 - h/2, w, h, _color_accent);

                _canvas->pushSprite(left + left_location - radius, top, TFT_TRANSPARENT);
                _canvas->pushSprite(left + left_location * 7 - radius, top, TFT_TRANSPARENT);

                _canvas->deleteSprite();
            }

            if (update_index >= 0)
            {
                // Top / Bottom regions
                // _canvas->createSprite(width, _top_height);
                // _canvas->setColorDepth(16);
                // _canvas->setSwapBytes(true);
                // _canvas->fillSprite(_color_header_footer_bg);

                // _canvas->setTextColor(TFT_WHITE);
                // _canvas->drawCentreString("44100 Hz", width/2, _top_height/2, 1);

                // _canvas->pushSprite(left, top);
                // _canvas->pushSprite(left, top + height - _top_height);

                // _canvas->deleteSprite();
            }
        }

        uint16_t cassette_height = height * 0.5;

    private:
        uint16_t _top_height;

        uint16_t _color_header_footer_bg = TFT_BLACK;

        uint16_t _color_cassette_bg = TFT_DARK_DARK_GRAY;
        uint16_t _color_tape = TFT_LIGHTGREY;
        uint16_t _color_cassette = TFT_DARK_GRAY;
        
        uint16_t _color_accent = TFT_RED;

        uint8_t _layer_to_update = 0;
};
