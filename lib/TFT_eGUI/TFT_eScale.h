#pragma once

#include <vector>
#include "TFT_eSPI_Ex.h"

enum TFT_eScale_Indicators {
    None = 0,
    Top = 1,
    Bottom = 2,
    Both = 3
};

struct TFT_eScale 
{
    int left = 0;
    int top = 0;

    int width = 100;
    int height = 20;

    int start_padding = 15;
    int interval_width = 2;

    int background_color = TFT_BLACK;
    int foreground_color = TFT_DARKGREY;

    TFT_eScale_Indicators interval_layout = Top;

    bool show_labels = true;
    bool show_intervals = true;
    bool show_sub_intervals = true;

    bool horizontal_labels = true;

    std::vector<int> _values;

    TFT_eSprite* canvas = NULL;

    TFT_eSprite* text_rotation = NULL;

    TFT_eScale(TFT_eSprite& canvas, const std::vector<int>& values, const char* label = NULL) 
        : canvas(&canvas), text_rotation(NULL), _values(values.begin(), values.end())
    { }

    TFT_eScale(TFT_eSprite& canvas, TFT_eSprite& text, const std::vector<int>& values, const char* label = NULL) 
        : canvas(&canvas), text_rotation(&text), _values(values.begin(), values.end())
    { }
};

void gui_scale_init(const TFT_eScale& scale) 
{
    scale.canvas->setColorDepth(16);
    scale.canvas->createSprite(scale.width, scale.height);
    scale.canvas->setSwapBytes(true);
    scale.canvas->fillSprite(scale.background_color);
}

int gui_scale_get_indicator_length_horizontal(const TFT_eScale& scale) {
    switch (scale.interval_layout) {
        case Top:
        case Bottom:
            return scale.height - scale.canvas->fontHeight() - 12;
        case Both:
            return (scale.height - scale.canvas->fontHeight())/ 2 - 12;
        default:
            break;
    }
    return 0;
}

// int gui_scale_get_indicator_length_vertical(const TFT_eScale& scale, int text_index) {
    
//     switch (scale.interval_layout) {
//         case 1:
//         case 2:
//             return scale.height - scale.canvas->fontHeight() - 12;
//         case 3:
//             return (scale.height - scale.canvas->fontHeight())/ 2 - 12;
//         return 0;
//     }
// }

int gui_scale_get_text_max_width(const TFT_eScale& scale, int text_index) {
    auto result = -1;
    for(const int &i : scale._values) {
        auto text = String(scale._values[i]);
        auto text_width = scale.canvas->textWidth(text) + 1;
        if (result < text_width) {
            result = text_width;
        }
    }
    return result;
}

int gui_scale_get_text_max_width(const TFT_eScale& scale, std::vector<String>& results) {
    auto result = -1;
    for(const int &i : scale._values) {
        auto text = String(scale._values[i]);
        auto text_width = scale.canvas->textWidth(text) + 1;
        if (result < text_width) {
            result = text_width;
        }
    }
    return result;
}

void gui_scale_get_text_values(const TFT_eScale& scale, std::vector<String>& results) {
    for(const int &i : scale._values) {
        auto text = String(i);
        results.push_back(text);
    }
}

int gui_scale_get_indicator_interval(const TFT_eScale& scale) {
    int marks_count = scale._values.size();
    if (scale.width > scale.height) {
        return (scale.width - scale.start_padding - (marks_count - 1) * scale.interval_width) / (marks_count - 1);
    }
    return (scale.height - scale.start_padding - (marks_count - 1) * scale.interval_width) / (marks_count - 1);
}

int gui_scale_get_text_center(const TFT_eScale& scale)
{
    if (!scale.show_intervals) {
        return scale.height / 2;
    }

    auto length =  gui_scale_get_indicator_length_horizontal(scale);
    auto padding = scale.height - length;

    switch (scale.interval_layout)
    {
        case Top:
            return (length + padding / 2);
        case Bottom:
            return (padding / 2);
        default:
            break;
    }

    return scale.height / 2;      
}

void gui_scale_begin(const TFT_eScale& scale) 
{
    not_null(scale.canvas);

    int mark_w = scale.interval_width;
    int marks_count = scale._values.size();

    int long_marks_interval = gui_scale_get_indicator_interval(scale);
    int long_marks_length = gui_scale_get_indicator_length_horizontal(scale);

    int long_start_padding = scale.start_padding;
    int short_start_padding = long_start_padding + long_marks_interval / 2;

    if (long_marks_length > 0 && scale.show_intervals) {
        for (auto i = 0; i < marks_count; i++)
        {
            if (scale.interval_layout == Top || scale.interval_layout == Both)
            {
                scale.canvas->drawWideLine(
                    long_start_padding + long_marks_interval*i - mark_w/2, 0, 
                    long_start_padding + long_marks_interval*i - mark_w/2, long_marks_length,
                    mark_w,
                    scale.foreground_color);
            }
            if (scale.interval_layout == Bottom || scale.interval_layout == Both)
            {
                scale.canvas->drawWideLine(
                    long_start_padding + long_marks_interval*i - mark_w/2, scale.height - long_marks_length, 
                    long_start_padding + long_marks_interval*i - mark_w/2, scale.height,
                    mark_w,
                    scale.foreground_color);
            }
        }
    }

    if (scale.show_labels) {
        
        scale.canvas->setTextColor(scale.foreground_color, scale.background_color);
        auto text_center = gui_scale_get_text_center(scale);
        auto text_h = scale.canvas->fontHeight() + 1;

        for (auto i = 0; i < marks_count; i++)
        {
            auto label = String(scale._values[marks_count - i - 1]);

            if (!scale.horizontal_labels && scale.text_rotation != NULL)
            {
                auto text_w = scale.canvas->textWidth(label) + 1;

                scale.text_rotation->setColorDepth(16);
                scale.text_rotation->createSprite(text_w, text_h);
                scale.text_rotation->setSwapBytes(true);
                scale.text_rotation->setTextColor(scale.foreground_color, scale.background_color);

                scale.text_rotation->drawString(label, 0, 0); //,1
                scale.canvas->setPivot(scale.start_padding + long_marks_interval * i - mark_w/2, text_center); //long_marks_length + text_h

                scale.text_rotation->pushRotated(scale.canvas, 90);
                scale.text_rotation->deleteSprite();
            }
            else
            {
                scale.canvas->drawCentreString(label, scale.start_padding + long_marks_interval * i - mark_w/2, text_center - text_h / 2);
            }
        }
    }

    if (long_marks_length > 1 && scale.show_sub_intervals) 
    {
        for (auto i = 0; i < marks_count - 1; i++)
        {
            if (scale.interval_layout == Top || scale.interval_layout == Both)
            {
                scale.canvas->drawWideLine(
                    short_start_padding + long_marks_interval * i - mark_w/2, 0, 
                    short_start_padding + long_marks_interval * i - mark_w/2, long_marks_length / 2,
                    mark_w,
                    scale.foreground_color);
            }
            if (scale.interval_layout == Bottom || scale.interval_layout == Both)
            {
                scale.canvas->drawWideLine(
                    short_start_padding + long_marks_interval * i - mark_w/2, scale.height - long_marks_length / 2, 
                    short_start_padding + long_marks_interval * i - mark_w/2, scale.height,
                    mark_w,
                    scale.foreground_color);
            }
        }
    }

    scale.canvas->pushSprite(scale.left, scale.top);
}