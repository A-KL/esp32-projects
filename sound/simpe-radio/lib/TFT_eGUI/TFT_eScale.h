#pragma once

#include <vector>
#include <TFT_eSPI.h> 

#include "TFT_eSPI_Ex.h"
#include "TFT_eWidget.h"

enum TFT_eScale_Indicators {
    None = 0,
    Top = 1,
    Bottom = 2,
    Both = 3
};

class TFT_eScale : public TFT_eWidget
{
    public:
        TFT_eScale(TFT_eSPI *tft, const std::vector<int>& values, const int16_t width, const int16_t height, int16_t top = 0, int16_t left = 0) 
            : TFT_eWidget(tft, width, height, top, left), _values(values.begin(), values.end()), _text_rotation(tft)
        {
        }

        int start_padding = 15;
        int interval_width = 2;

        int background_color = TFT_BLACK;
        int foreground_color = TFT_DARKGREY;

        TFT_eScale_Indicators interval_layout = Top;

        bool show_labels = true;
        bool show_intervals = true;
        bool show_sub_intervals = true;

        bool horizontal_labels = true;

        void init()
        {
            create(width, height, background_color);
        }

        void load_font(const uint8_t array[])
        {
            TFT_eWidget::load_font(array);
            _text_rotation.loadFont(array);
        }

        void update() {}

        void begin() 
        {
            auto mark_w = interval_width;
            auto marks_count = _values.size();

            auto long_marks_interval = get_indicator_interval();
            auto long_marks_length = get_indicator_length_horizontal();

            auto long_start_padding = start_padding;
            auto short_start_padding = long_start_padding + long_marks_interval / 2;

            if (long_marks_length > 0 & show_intervals) 
            {
                for (auto i = 0; i < marks_count; i++)
                {
                    if (interval_layout == Top || interval_layout == Both)
                    {
                        _canvas.drawWideLine(
                            long_start_padding + long_marks_interval*i - mark_w/2, 0, 
                            long_start_padding + long_marks_interval*i - mark_w/2, long_marks_length,
                            mark_w,
                            foreground_color);
                    }

                    if (interval_layout == Bottom || interval_layout == Both)
                    {
                        _canvas.drawWideLine(
                            long_start_padding + long_marks_interval*i - mark_w/2, height - long_marks_length, 
                            long_start_padding + long_marks_interval*i - mark_w/2, height,
                            mark_w,
                            foreground_color);
                    }
                }
            }

            if (show_labels) 
            {
                _canvas.setTextColor(foreground_color, background_color);
                
                auto text_center = get_text_center();
                auto text_h = _canvas.fontHeight() + 1;

                for (auto i = 0; i < marks_count; i++)
                {
                    auto label = String(_values[marks_count - i - 1]);

                    if (!horizontal_labels)
                    {
                        auto text_w = _canvas.textWidth(label) + 1;

                        _text_rotation.setColorDepth(16);
                        _text_rotation.createSprite(text_w, text_h);
                        _text_rotation.setSwapBytes(true);
                        _text_rotation.setTextColor(foreground_color, background_color);

                        _text_rotation.drawString(label, 0, 0, 1);
                        _canvas.setPivot(start_padding + long_marks_interval * i - mark_w/2, text_center); //long_marks_length + text_h

                        _text_rotation.pushRotated(&_canvas, 90, background_color);
                        _text_rotation.deleteSprite();
                    }
                    else
                    {
                        _canvas.drawCentreString(label, start_padding + long_marks_interval * i - mark_w/2, text_center - text_h / 2, 1);
                    }
                }
            }

            if (long_marks_length > 1 & show_sub_intervals) 
            {
                for (auto i = 0; i < marks_count - 1; i++)
                {
                    if (interval_layout == Top || interval_layout == Both)
                    {
                        _canvas.drawWideLine(
                            short_start_padding + long_marks_interval * i - mark_w/2, 0, 
                            short_start_padding + long_marks_interval * i - mark_w/2, long_marks_length / 2,
                            mark_w,
                            foreground_color);
                    }
                    if (interval_layout == Bottom || interval_layout == Both)
                    {
                        _canvas.drawWideLine(
                            short_start_padding + long_marks_interval * i - mark_w/2, height - long_marks_length / 2, 
                            short_start_padding + long_marks_interval * i - mark_w/2, height,
                            mark_w,
                            foreground_color);
                    }
                }
            }

            push(left, top);
        }

    private:
        std::vector<int> _values;
        TFT_eSprite _text_rotation;

        int get_indicator_length_horizontal() 
        {
            switch (interval_layout) {
                case Top:
                case Bottom:
                    return height - _canvas.fontHeight() - 12;
                case Both:
                    return (height - _canvas.fontHeight())/ 2 - 12;
            }
            return 0;
        }

        int get_text_max_width(int text_index) 
        {
            auto result = -1;
            for(const int &i : _values) {
                auto text = String(_values[i]);
                auto text_width = _canvas.textWidth(text) + 1;
                if (result < text_width) {
                    result = text_width;
                }
            }
            return result;
        }

        int get_text_center()
        {
            if (!show_intervals) {
                return height / 2;
            }

            auto length =  get_indicator_length_horizontal();
            auto padding = height - length;

            switch (interval_layout)
            {
                case Top:
                    return (length + padding / 2);
                case Bottom:
                    return (padding / 2);
            }

            return height / 2;      
        }

        int get_indicator_interval() 
        {
            auto marks_count = _values.size();
            if (width > height) {
                return (width - start_padding - (marks_count - 1) * interval_width) / (marks_count - 1);
            }
            return (height - start_padding - (marks_count - 1) * interval_width) / (marks_count - 1);
        }

        void get_text_values(std::vector<String>& results) {
            for(const int &i : _values) {
                auto text = String(_values[i]);
                results.push_back(text);
            }
        }

        // int get_text_max_width(std::vector<String>& results) {
        //     auto result = -1;
        //     for(const int &i : _values) {
        //         auto text = String(_values[i]);
        //         auto text_width = canvas.textWidth(text) + 1;
        //         if (result < text_width) {
        //             result = text_width;
        //         }
        //     }
        //     return result;
        // }
};

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
