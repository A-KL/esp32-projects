#pragma once

#include "TFT_eSPI_Ex.h"
#include "TFT_eColorBrush.h"

#include "TFT_eProgressBar_ValueStyle.h"

class TFT_eProgressBar : public TFT_eWidget
{
    public:
        TFT_eProgressBar(TFT_eSPI* tft, const TFT_eProgressBar_ValueStyle* style, int16_t left = 0, const int16_t top = 0, const int16_t width = 200, const int16_t height = 20)
        : TFT_eWidget(tft, width, height, left, top), value_style(style)
        { }

    int borders_thickness[4] = {0, 0, 0, 0};

    float value = 50;
    float min = 0;
    float max = 100;

    uint16_t background_color = 0x39C7;
    uint16_t border_color = TFT_WHITE;

    //value_styleTFT_eProgressBar_ValueStyle* bar_style = NULL;
    const TFT_eProgressBar_ValueStyle* value_style = NULL;

    inline void init()
    {   
        create(width, height, background_color);
    }

    void update() 
    {
        auto left_act= border_with_padding(borders_thickness[0], border_padding);
        auto top_act = border_with_padding(borders_thickness[1], border_padding);
        auto right_act = border_with_padding(borders_thickness[2], border_padding);
        auto bottom_act = border_with_padding(borders_thickness[3], border_padding);

        auto w = width - left_act - right_act;
        auto h = height - top_act - bottom_act;

        _canvas.fillRect(left_act, top_act, w, h, background_color);

        int value_w = fmap(value, min, max, 0, w);

        value_style->render(&_canvas, left_act, top_act, w, h, value_w);
        
        push();
    }

    void begin() 
    {
        if (borders_thickness[0] > 0) {
            _canvas.drawWideLine(0, 0, 0, 0 + height, 
                borders_thickness[0], 
                border_color);
        }

        if (borders_thickness[1] > 0) {
            _canvas.drawWideLine(0, 0, 0 + width, 0, 
                borders_thickness[1], 
                border_color);
        }

        if (borders_thickness[2] > 0) {
            _canvas.drawWideLine(0 + width - 1, 0, 0 + width - 1, 0 + height, 
                borders_thickness[2], 
                border_color);
        }

        if (borders_thickness[3] > 0) {
            _canvas.drawWideLine(0, 0 + height - 1, 1 + width, 0 + height - 1, 
                borders_thickness[3], 
                border_color);
        }

        update();
    }

    private:
        const int border_padding = 1;
};