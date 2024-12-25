#pragma once

#include <vector>

#include <TFT_eSPI.h> 

#include "TFT_eSPI_Ex.h"

class TFT_eLabel : public TFT_eWidget
{
    public:
        TFT_eLabel(TFT_eSPI* tft, const char* text, int16_t border = 0, int16_t color = TFT_DARKGREY, int16_t top = 0, int16_t left = 0, const int16_t width = -1, const int16_t height = -1) 
        : TFT_eWidget(tft, width, height, top, left), _text(text), borders_thickness{border, border, border, border}, foreground_color(color)
        { }

        int foreground_color = TFT_DARKGREY;
        int background_color = TFT_BLACK;

        int borders_thickness[4] = {0, 0, 0, 0};

        inline void init() 
        {
            height = _canvas.fontHeight();
            height += borders_thickness[1];
            height += borders_thickness[3];
            height += default_padding_h;

            width = _canvas.textWidth(_text);
            width += borders_thickness[0];
            width += borders_thickness[2];
            width += default_padding_w;

            _canvas.deleteSprite();
            _canvas.setColorDepth(16);
            _canvas.createSprite(width, height);
            _canvas.setSwapBytes(true);

            create(width, height, background_color);
        }

        void update()
        {
            _canvas.fillSprite(background_color);

            if (borders_thickness[0] > 0) {
                _canvas.drawWideLine(0, 0, 0, 0 + height, 
                    borders_thickness[0], 
                    foreground_color);
            }

            if (borders_thickness[1] > 0) {
                _canvas.drawWideLine(0, 0, 0 + width, 0, 
                    borders_thickness[1], 
                    foreground_color);
            }

            if (borders_thickness[2] > 0) {
                _canvas.drawWideLine(0 + width - 1, 0, 0 + width - 1, 0 + height, 
                    borders_thickness[2], 
                    foreground_color);
            }

            if (borders_thickness[3] > 0) {
                _canvas.drawWideLine(0, 0 + height - 1, 1 + width, 0 + height - 1, 
                    borders_thickness[3], 
                    foreground_color);
            }

            auto text_string = String(_text);

            _canvas.setTextColor(foreground_color);
            _canvas.drawCentreString(text_string, width/2, default_padding_h + borders_thickness[1], 1);
            
            _canvas.pushSprite(left, top);
        }

    private:
        const char* _text;

        const int default_padding_w = 2;
        const int default_padding_h = 2;
};