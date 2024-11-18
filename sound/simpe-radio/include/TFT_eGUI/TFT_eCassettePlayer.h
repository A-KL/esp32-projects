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

    protected:
        TFT_eSprite _canvas;
        TFT_eSprite _canvas_to_rotate;
};

class TFT_eCassettePlayer : public TFT_eWidget
{
    public:
        TFT_eCassettePlayer(TFT_eSPI *tft, int16_t width, int16_t height, int16_t top = 0, int16_t left = 0) 
        : TFT_eWidget(tft, width, height, top, left)
        { }

        void init()
        {   
            _top_height = (height - cassette_height) / 2;
        }

        void update()
        {
            draw("ESP32 MP3 PLAYER", "LOADING...", 3, 0);
        }

        void draw(const char* top_text, const char* bottom_text, int16_t update_index = 3, uint16_t angle = 0)
        {
            // Background
            if (update_index >= 3)
            {
                _canvas.createSprite(width, cassette_height);
                _canvas.setColorDepth(16);
                _canvas.setSwapBytes(true);
                _canvas.fillSprite(_color_cassette_bg);
                _canvas.pushSprite(left, top + _top_height);
                _canvas.deleteSprite();
            }

            auto outer_diameter = height;
            auto tape_diameter = outer_diameter * 0.8;
            auto radius = outer_diameter / 2;

            auto left_location = width / 6;
            auto right_location = left_location * 5;

            // Cassette
            if (update_index >= 2)
            {
                _canvas.createSprite(outer_diameter, outer_diameter);
                _canvas.setColorDepth(16);
                _canvas.setSwapBytes(true);
                _canvas.fillSprite(TFT_TRANSPARENT);

                _canvas.fillSmoothCircle(radius, radius, radius, _color_cassette);
                _canvas.fillSmoothCircle(radius, radius, tape_diameter/2.5, _color_tape);
                _canvas.fillSmoothCircle(radius, radius, tape_diameter/3, TFT_BLACK);
                _canvas.fillSmoothCircle(radius, radius, tape_diameter/3.2, _color_accent);

                //_canvas->fillSmoothCircle(radius, radius, tape_diameter/5, TFT_BLACK);

                _canvas.pushSprite(left + left_location - radius, top, TFT_TRANSPARENT);
                _canvas.pushSprite(left + right_location - radius, top, TFT_TRANSPARENT);

                _canvas.deleteSprite();
            }

            if (update_index >= 1)
            {
                auto sprite_w = tape_diameter / 2;
                auto h = sprite_w / 7;
                auto w = sprite_w / 4;
                auto r = sprite_w / 2;

                _canvas.createSprite(sprite_w, sprite_w);
                _canvas.setColorDepth(16);
                _canvas.setSwapBytes(true);

                _canvas.fillSprite(TFT_TRANSPARENT);
                _canvas.fillSmoothCircle(r, r, tape_diameter / 5, TFT_BLACK);

                // Notches
                _canvas_to_rotate.createSprite(sprite_w, sprite_w);
                _canvas_to_rotate.setColorDepth(16);
                _canvas_to_rotate.setSwapBytes(true);
                _canvas_to_rotate.fillSprite(TFT_TRANSPARENT);

                _canvas_to_rotate.fillRect(0, r - h/2, w, h, _color_accent); // Left notch
                _canvas_to_rotate.fillRect(sprite_w-w, r - h/2, w, h, _color_accent); // Right notch

                _canvas.setPivot(r, r);

                _canvas_to_rotate.pushRotated(&_canvas, angle, TFT_TRANSPARENT);
                _canvas_to_rotate.pushRotated(&_canvas, angle + 60, TFT_TRANSPARENT);
                _canvas_to_rotate.pushRotated(&_canvas, angle + 120, TFT_TRANSPARENT);

                _canvas_to_rotate.deleteSprite();

                _canvas.pushSprite(left + left_location - r, top + height/2 - r, TFT_TRANSPARENT);
                _canvas.pushSprite(left + right_location - r, top + height/2 - r, TFT_TRANSPARENT);

                _canvas.deleteSprite();
            }

            // Top / Bottom regions
            if (update_index >= 0)
            {
                _canvas.createSprite(width, _top_height);
                _canvas.setColorDepth(16);
                _canvas.setSwapBytes(true);
                _canvas.setTextColor(TFT_WHITE);

                _canvas.fillSprite(_color_header_footer_bg);
                _canvas.drawCentreString(top_text, width/2, _top_height/2, 1);
                _canvas.pushSprite(left, top);

                _canvas.fillSprite(_color_header_footer_bg);
                _canvas.drawCentreString(bottom_text, width/2, _top_height/2, 1);
                _canvas.pushSprite(left, top + height - _top_height);

                _canvas.deleteSprite();
            }
        }

        uint16_t cassette_height = height * 0.7;

    private:
        uint16_t _top_height;

        uint16_t _color_header_footer_bg = TFT_BLACK;

        uint16_t _color_cassette_bg = TFT_DARK_DARK_GRAY;
        uint16_t _color_tape = TFT_LIGHTGREY;
        uint16_t _color_cassette = TFT_DARK_GRAY;
        
        uint16_t _color_accent = TFT_RED;

        uint8_t _layer_to_update = 0;
};
