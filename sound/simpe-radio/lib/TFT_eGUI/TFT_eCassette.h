#pragma once

#include <SPI.h>
#include <TFT_eSPI.h>

#include "TFT_eSPI_Ex.h"
#include "TFT_eWidget.h"

class TFT_eCassette : public TFT_eWidget
{
    public:
        TFT_eCassette(TFT_eSPI *tft, int16_t width, int16_t height, int16_t top = 0, int16_t left = 0) 
            : TFT_eWidget(tft, width, height, top, left), _canvas_to_rotate(tft), _outer_diameter(height)
        {
        }

        inline void init()
        { 
            auto sprite_w = _outer_diameter / 2;
            auto h = sprite_w / 14;
            auto w = sprite_w / 5;
            auto r = sprite_w / 2;

            _canvas_to_rotate.createSprite(sprite_w, sprite_w);
            _canvas_to_rotate.setColorDepth(16);
            _canvas_to_rotate.setSwapBytes(true);
            _canvas_to_rotate.fillSprite(TFT_TRANSPARENT);

            _canvas_to_rotate.fillRect(0, r - h/2, w, h, _color_accent); // Left notch
            _canvas_to_rotate.fillRect(sprite_w-w, r - h/2, w, h, _color_accent); // Right notch
        }

        void load_font(const uint8_t array[])
        {
            TFT_eWidget::load_font(array);

            _canvas_to_rotate.loadFont(array);
        }

        inline void set_color(const uint16_t color)
        {
            if (_color_accent == color) {
                return;
            }
            _color_accent = color;
            partial_update(2);
        }

        inline void rotate_tape(uint16_t angle)
        {
            if (_angle == angle) {
                return;
            }
            _angle = angle;
            partial_update(1);
        }

        // inline void set_tape(float proportion)
        // {
        //     if (_proportion == proportion) {
        //         return;
        //     }
        //     _proportion = proportion;
        //     partial_update(2);
        // }

        inline void set_header(const char* text)
        {
            _top_text = text; // TOOO: copy
            partial_update(0);
        }

        inline void set_footer(const char* text)
        {
            _bottom_text = text; // TOOO: copy
            partial_update(0);
        }

        inline void set_params(const char* format, const char* req)
        {
            _format_text = format; // TOOO: copy
            _freq_text = req; // TOOO: copy
            partial_update(0);
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

            auto tape_diameter = _outer_diameter * 0.8;
            auto radius = _outer_diameter / 2;

            auto left_location = width / 6;
            auto right_location = left_location * 5;

            // Cassette
            if (_update_index >= 2)
            {
                auto canvas = create(_outer_diameter, _outer_diameter, TFT_TRANSPARENT);

                canvas->fillSmoothCircle(radius, radius, radius, _color_cassette);
                canvas->fillSmoothCircle(radius, radius, tape_diameter/2, _color_tape);
                canvas->fillSmoothCircle(radius, radius, tape_diameter/3, TFT_BLACK);
                canvas->fillSmoothCircle(radius, radius, tape_diameter/3.2, _color_accent);

                push_transparent(left + left_location - radius, top);
                push_transparent(left + right_location - radius, top);

                del();
            }

            // Moving cassette parts
            if (_update_index >= 1)
            {
                auto sprite_w = _outer_diameter / 2;
                auto r = sprite_w / 2;

                auto canvas = create(sprite_w, sprite_w, TFT_TRANSPARENT);

                canvas->fillSmoothCircle(r, r, tape_diameter / 4, TFT_BLACK);

                // Notches
                canvas->setPivot(r, r);

                _canvas_to_rotate.pushRotated(&_canvas, _angle, TFT_TRANSPARENT);
                _canvas_to_rotate.pushRotated(&_canvas, _angle + 60, TFT_TRANSPARENT);
                _canvas_to_rotate.pushRotated(&_canvas, _angle + 120, TFT_TRANSPARENT);

                push_transparent(left + left_location - r, top + height/2 - r);
                push_transparent(left + right_location - r, top + height/2 - r);

                del();
            }

            // Header / Footer
            if (_update_index >= 0)
            {
                auto canvas = create(width, cassette_header, _color_header_footer_bg);
                canvas->setTextColor(TFT_WHITE);

                auto text_top = cassette_header/2 - canvas->fontHeight()/2;
                auto text_left = width/2;

                canvas->drawCentreString(_top_text, text_left, text_top, 1);
                push(left, top);

                canvas->fillSprite(_color_header_footer_bg);
                canvas->drawCentreString(_bottom_text, text_left, text_top, 1);
                push(left, top +  cassette_header + cassette_height + 1);

                del();
            }

            // Middle
            if (_update_index >= 0)
            {
                auto sprite_w = width / 6;
                auto sprite_h = cassette_height / 2;
                auto sprite_left = width / 2 - sprite_w / 2;
                auto text_left = sprite_w/2;

                auto canvas = create(sprite_w, sprite_h, TFT_TRANSPARENT);
                canvas->setTextColor(_color_tape);

                auto text_top = sprite_h / 2 - canvas->fontHeight() / 2;

                canvas->drawCentreString(_format_text, text_left, text_top, 1);
                push_transparent(left + sprite_left, top + cassette_header);

                canvas->fillSprite(TFT_TRANSPARENT);

                canvas->drawCentreString(_freq_text, text_left, text_top, 1);
                push_transparent(left + sprite_left, top + cassette_header + sprite_h);

                del();
            }

            _update_index = -1;
        }

        float cassette_height_ratio = 0.6;

    private:
        TFT_eSprite _canvas_to_rotate;

        const char* _top_text = "ESP32 MP3 PLAYER";
        const char* _bottom_text = "LOADING...";

        const char* _format_text = "PCM";
        const char* _freq_text = "48K";
        
        uint16_t _outer_diameter;
        uint16_t _angle = 0;

        uint16_t _color_header_footer_bg = TFT_BLACK;
        uint16_t _color_accent = TFT_RED;
        uint16_t _color_accent_text = TFT_RED;
        uint16_t _color_cassette_bg = TFT_DARK_DARK_GRAY;
        uint16_t _color_tape = TFT_DARKGREY;
        uint16_t _color_cassette = TFT_DARK_GRAY;
        
        int8_t _update_index = 3;

        void partial_update(uint8_t index)
        {
            if (_update_index < index) {
                _update_index = index;
            }
        }
};
