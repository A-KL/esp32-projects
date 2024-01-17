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

    int start_padding = 15;

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

    int mark_w = 2;
    int marks_count = scale._values.size();

    int long_marks_interval = (scale.width - scale.start_padding * 1 - (marks_count - 1) * mark_w) / (marks_count - 1);
    
    int long_start_padding = scale.start_padding;
    int short_start_padding = long_start_padding + long_marks_interval / 2;

    int font_height = scale.show_labels ? scale.canvas->fontHeight() : 0;
    int long_marks_height = scale.show_marks ? scale.height - scale.canvas->fontHeight() - 1 : 0;

    if (scale.show_marks && long_marks_height > 0) {
        for (auto i = 0; i < marks_count; i++)
        {
            scale.canvas->drawWideLine(
                long_start_padding + long_marks_interval*i - mark_w/2, 0, 
                long_start_padding + long_marks_interval*i - mark_w/2, long_marks_height,
                mark_w,
                scale.foreground_color);
                //log_w("i: %d, x: %d ", i, scale.start_padding + long_marks_interval*i);
        }
    }

    if (scale.show_labels) {
        scale.canvas->setTextColor(scale.foreground_color, scale.background_color);

        for (auto i = 0; i < marks_count; i++)
        {
            auto label = String(scale._values[marks_count - i - 1]);
            scale.canvas->drawCentreString(label, scale.start_padding + long_marks_interval * i - 1, long_marks_height + 3, 1);
        }
    }

    if (scale.show_marks) {
        for (auto i = 0; i < marks_count - 1; i++)
        {
            scale.canvas->drawWideLine(
                short_start_padding + long_marks_interval * i - mark_w/2, 0, 
                short_start_padding + long_marks_interval * i - mark_w/2, long_marks_height / 2,
                mark_w,
                scale.foreground_color);
        }
    }

    scale.canvas->pushSprite(scale.left, scale.top);
}