#pragma once

#include <vector>

#include <TFT_eSPI.h> 

#include "TFT_eSPI_Ex.h"

struct TFT_eScale 
{
    int left = 0;
    int top = 0;

    int width = 100;
    int height = 20;

    int background_color = TFT_BLACK;
    int foreground_color = TFT_DARKGREY;

    bool show_marks = true;
    bool show_labels = true;

    std::vector<int> _values;

    TFT_eSprite* canvas = NULL;

    TFT_eScale(TFT_eSprite& canvas, const std::vector<int>& values, const char* label = NULL) 
        : canvas(&canvas), _values(values.begin(), values.end())
    { }
};

void gui_scale_init(const TFT_eScale& scale) 
{
    scale.canvas->setColorDepth(16);
    scale.canvas->createSprite(scale.width, scale.height);
    scale.canvas->setSwapBytes(true);
    scale.canvas->fillSprite(scale.background_color);
}

void gui_scale_begin(const TFT_eScale& scale) 
{
    not_null(scale.canvas);

    int long_marks = scale._values.size();
    int long_marks_padding = scale.width / long_marks;
    int start_padding = long_marks_padding/2;

    int font_height = scale.show_labels ? scale.canvas->fontHeight() : 0;
    int long_marks_height = scale.show_marks ? scale.height - scale.canvas->fontHeight() - 1: 0;

    if (scale.show_marks && long_marks_height > 0) {
        for (auto i = 0; i < long_marks; i++)
        {
            scale.canvas->drawWideLine(
                start_padding + long_marks_padding*i, 0, 
                start_padding + long_marks_padding*i, long_marks_height,
                2,
                scale.foreground_color);
        }
    }

    if (scale.show_labels) {
        scale.canvas->setTextColor(scale.foreground_color, scale.background_color);

        for (auto i = 0; i < long_marks; i++)
        {
            auto label = String(scale._values[long_marks - i - 1]);
            scale.canvas->drawCentreString(label, start_padding + long_marks_padding * i, long_marks_height + 3, 1);
        }
    }

    if (scale.show_marks)  {
        for (auto i = 0; i < long_marks - 1; i++)
        {
            scale.canvas->drawWideLine(
                start_padding * 2 + long_marks_padding * i, 0, 
                start_padding * 2 + long_marks_padding * i, long_marks_height / 2,
                2,
                scale.foreground_color);
        }
    }

    scale.canvas->pushSprite(scale.left, scale.top);
}