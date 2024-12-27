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
        uint16_t padding = 0;

        uint32_t on_color = TFT_GREENYELLOW;
        uint32_t on_color_to = TFT_GREEN;

        uint32_t off_color = TFT_DARKGREEN;

        uint32_t bg_color = TFT_DARK_GRAY;
        uint32_t bg_color_to = TFT_DARK_DARK_GRAY;

        inline void init()
        {
            create(width, height, round ? TFT_TRANSPARENT : bg_color);
        }

        void begin()
        {
            if (round) 
            {
                auto center = width / 2;
                _canvas.fillSmoothCircle(center, center, center - 1, bg_color);
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
            auto center = width / 2;
            auto r = (width - padding) / 2 - 1;

            _canvas.fillSmoothCircle(center, center, r, checked ? on_color : off_color);

            push_transparent(left, top);
        }

        inline void update_rect()
        {
            auto left = padding / 2;
            auto led_size = width - padding;

            if (bg_color == bg_color_to) 
            {
                _canvas.fillRect(
                    left, left, 
                    led_size, led_size, 
                    checked ? on_color : off_color);
            } 
            else 
            {
                _canvas.fillRectVGradient(
                    left, left, 
                    led_size, led_size, 
                    checked ? on_color : off_color,
                    checked ? on_color_to : off_color);
            }
            push();
        }
};

