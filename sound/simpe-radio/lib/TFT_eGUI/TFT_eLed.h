#pragma once

#include <TFT_eSPI.h> 
#include "TFT_eSPI_Ex.h"

class TFT_eLed : public TFT_eWidget
{
    public:
        TFT_eLed(TFT_eSPI* tft, uint16_t left = 0, uint16_t top = 0, uint16_t width = 20)
        : TFT_eWidget(tft, width, width, top, left)
        {}

        bool round = true;
        bool checked = false;

        uint32_t on_color = TFT_GREENYELLOW;
        uint32_t on_color_to = TFT_GREEN;

        uint32_t off_color = TFT_DARKGREEN;

        uint32_t bg_color = TFT_WHITE;
        uint32_t bg_color_to = TFT_DARKGREY;

        inline void init() 
        {
            create(width, height, round ? TFT_TRANSPARENT : bg_color);
        }

        void begin()
        {
            if (round) 
            {
                auto center = width / 2;
                _canvas.fillSmoothCircle(center, center, width / 2 - 1, bg_color);
            } 
            else 
            {
                if (bg_color == bg_color_to)
                    _canvas.fillScreen(bg_color);
                else
                    _canvas.fillRectVGradient(0, 0, width, width, bg_color, bg_color_to);
            }

            update();
        }

        void update() 
        {
            if (round) 
                update_round();
            else
                update_rect();
        }

    private:
        inline void update_round() 
        {
            auto padding = width / 10;
            auto center = width / 2;

            if (padding <= 0)
                padding = 1;

            auto r = (width - padding) / 2 - 1;

            _canvas.fillCircle(center, center, r, checked ? on_color : off_color);

            push_transparent(left, top);
        }

        inline void update_rect()
        {
            auto padding = width / 10;

            if (padding <= 0)
                padding = 1;

            auto led_size = width - padding * 2;

            if (bg_color == bg_color_to) 
            {
                _canvas.fillRect(
                    padding, padding, 
                    led_size, led_size, 
                    checked ? on_color : off_color);
            } 
            else 
            {
                _canvas.fillRectVGradient(
                    padding, padding, 
                    led_size, led_size, 
                    checked ? on_color : off_color,
                    checked ? on_color_to : off_color);
            }
            push(left, top);
        }
};

