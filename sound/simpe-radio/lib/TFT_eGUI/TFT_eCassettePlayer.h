#pragma once

#include <SPI.h>
#include <TFT_eSPI.h>

#include "TFT_eSPI_Ex.h"
#include "TFT_eWidget.h"

class TFT_eCassettePlayer : public TFT_eWidget
{
    public:
        TFT_eCassettePlayer(TFT_eSPI *tft, int16_t width, int16_t height, int16_t top = 0, int16_t left = 0) 
        : TFT_eWidget(tft, width, height, top, left)
        { }

        inline void init()
        { }

        inline void rotate_tape(uint16_t angle)
        {
            _angle = angle;
            partial_update(1);
        }

        inline void set_color(const uint16_t color)
        {
            _color_accent = color;
            partial_update(2);
        }

        inline void set_header(const char* text)
        {
            _top_text = text; // TOOO: cop
            partial_update(1);
        }

        void update()
        {
            auto cassette_height = height * cassette_height_ratio;
            auto cassette_header = (height - cassette_height) / 2;
        
            // Background
            if (_update_index >= 3)
            {
                create(width, cassette_height, _color_cassette_bg);
                push(left, top + cassette_header);
                del();
            }

            auto outer_diameter = height;
            auto tape_diameter = outer_diameter * 0.8;
            auto radius = outer_diameter / 2;

            auto left_location = width / 6;
            auto right_location = left_location * 5;

            // Cassette
            if (_update_index >= 2)
            {
                auto canvas = create(outer_diameter, outer_diameter, TFT_TRANSPARENT);

                canvas->fillSmoothCircle(radius, radius, radius, _color_cassette);
                canvas->fillSmoothCircle(radius, radius, tape_diameter/2.5, _color_tape);
                canvas->fillSmoothCircle(radius, radius, tape_diameter/3, TFT_BLACK);
                canvas->fillSmoothCircle(radius, radius, tape_diameter/3.2, _color_accent);

                push_transparent(left + left_location - radius, top);
                push_transparent(left + right_location - radius, top);

                del();
            }

            // Moving cassette parts
            if (_update_index >= 1)
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

                _canvas_to_rotate.pushRotated(&_canvas, _angle, TFT_TRANSPARENT);
                _canvas_to_rotate.pushRotated(&_canvas, _angle + 60, TFT_TRANSPARENT);
                _canvas_to_rotate.pushRotated(&_canvas, _angle + 120, TFT_TRANSPARENT);

                _canvas_to_rotate.deleteSprite();

                _canvas.pushSprite(left + left_location - r, top + height/2 - r, TFT_TRANSPARENT);
                _canvas.pushSprite(left + right_location - r, top + height/2 - r, TFT_TRANSPARENT);

                _canvas.deleteSprite();
            }

            // Header / Footer
            if (_update_index >= 0)
            {
                _canvas.createSprite(width, cassette_header);
                _canvas.setColorDepth(16);
                _canvas.setSwapBytes(true);
                _canvas.setTextColor(TFT_WHITE);

                _canvas.fillSprite(_color_header_footer_bg);
                _canvas.drawCentreString(_top_text, width/2, cassette_header/2, 1);
                _canvas.pushSprite(left, top);

                _canvas.fillSprite(_color_header_footer_bg);
                _canvas.drawCentreString(_bottom_text, width/2, cassette_header/2, 1);
                _canvas.pushSprite(left, top + height - cassette_header);

                _canvas.deleteSprite();
            }

            _update_index = -1;
        }

        float cassette_height_ratio = 0.7;

    private:
        const char* _top_text = "ESP32 MP3 PLAYER";
        const char* _bottom_text = "LOADING...";
        
        uint16_t _angle = 0;

        uint16_t _color_header_footer_bg = TFT_BLACK;

        uint16_t _color_cassette_bg = TFT_DARK_DARK_GRAY;
        uint16_t _color_tape = TFT_LIGHTGREY;
        uint16_t _color_cassette = TFT_DARK_GRAY;
        
        uint16_t _color_accent = TFT_RED;

        uint8_t _update_index = 0;

        void partial_update(uint8_t index)
        {
            if (_update_index < index) {
                _update_index = index;
            }
        }
};
