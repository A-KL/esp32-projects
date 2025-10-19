#pragma once

#include "TFT_eSPI_Ex.h"
#include "TFT_eColorBrush.h"

class TFT_ePanel : public TFT_eWidget
{
    public:
        TFT_ePanel(TFT_eSPI* tft, const TFT_eColorBrush* bg_brush, int16_t top = 0, int16_t left = 0, const int16_t width = 40, const int16_t height = 20) 
        : TFT_eWidget(tft, width, height, top, left), background(bg_brush)
        { }

        const TFT_eColorBrush* background = NULL;

        inline void init() {
            create(width, height, TFT_BLACK);
        }

        inline void update() 
        {
            if (background == NULL) {
                return;
            }

            background->fillRect(_canvas, 0, 0, width, height);
            push();
        }
};