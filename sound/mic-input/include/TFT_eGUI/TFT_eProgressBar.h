#pragma once

#include <map>
#include <vector>

#include <TFT_eSPI.h>

#include "TFT_eSPI_Ex.h"
#include "TFT_eColorBrush.h"

class TFT_eProgressBar_ValueStyle {
    public:
        virtual void render(TFT_eSprite* sprite, int left, int top, int w, int h, int value_w) const = 0;
};

class TFT_eProgressBar_SimpleValueStyle : public TFT_eProgressBar_ValueStyle 
{
    public:
        TFT_eProgressBar_SimpleValueStyle(const TFT_eColorBrush* value_color) 
            : _value_color(value_color)
            { } 

        TFT_eProgressBar_SimpleValueStyle(const TFT_eColorBrush& color_brush) 
            : TFT_eProgressBar_SimpleValueStyle(&color_brush)
            { } 

        virtual void render(TFT_eSprite* sprite, int left, int top, int w, int h, int value_w) const 
        {
            _value_color->fillRect(sprite, left, top, value_w, h);
        };

    private:      
        const TFT_eColorBrush* _value_color;
};

class TFT_eProgressBar_SegmentedValueStyle : public TFT_eProgressBar_ValueStyle 
{
    public:
        TFT_eProgressBar_SegmentedValueStyle(
            const TFT_eColorBrush* segment_color,
            const TFT_eColorBrush* segment_hi_color,
            const TFT_eColorBrush* segment_bg_color,
            const TFT_eColorBrush* segment_hi_bg_color, 
            const int padding, 
            const int segments_count) 
            :  _segment_padding(padding),
               _segments_count(segments_count),
               _thresholds({ 
                    { 0, segment_color }, 
                    { (segments_count * 0.9), segment_hi_color }
                }),
                _bg_thresholds({ 
                        { 0, segment_bg_color }, 
                        { (segments_count * 0.9), segment_hi_bg_color }
                    })
        { }

        TFT_eProgressBar_SegmentedValueStyle(
            const TFT_eColorBrush& segment_color,
            const TFT_eColorBrush& segment_hi_color,
            const TFT_eColorBrush& segment_bg_color,
            const TFT_eColorBrush& segment_hi_bg_color,
            const int padding, 
            const int segments_count)
            : TFT_eProgressBar_SegmentedValueStyle(
                &segment_color,
                &segment_hi_color,
                &segment_bg_color,
                &segment_hi_bg_color,
                padding, 
                segments_count)
        { } 

        virtual void render(TFT_eSprite* sprite, int left, int top, int w, int h, int value_w) const 
        {
            auto segment_w = w / _segments_count - _segment_padding;
            auto value_segments_count = value_w / segment_w;

            auto value_color = _thresholds.begin()->second;
            auto background_color = _bg_thresholds.begin()->second;

            for (auto i=0; i<value_segments_count; i++) 
            {
                auto segment_left = left + i * (segment_w + _segment_padding);
                value_color->fillRect(sprite, segment_left, top, segment_w, h);

                auto results = _thresholds.find(i);
                if (results != _thresholds.end()) {
                    value_color = results->second;
                }
            }

            for (auto i=value_segments_count; i<_segments_count + 1; i++) 
            {
                auto segment_left = left + i * (segment_w + _segment_padding);
                background_color->fillRect(sprite, segment_left, top, segment_w, h);

                auto results = _bg_thresholds.find(i);
                if (results != _bg_thresholds.end()) {
                    background_color = results->second;
                }
            }
        };

    private:
        const int _segment_padding;
        const int _segments_count;

        std::map<int, const TFT_eColorBrush*> _thresholds;
        std::map<int, const TFT_eColorBrush*> _bg_thresholds;
};

struct TFT_eProgressBar
{
    int left = 0;
    int top = 0;
    int height = 20;
    int width = 100;

    int borders_thickness[4] = {0, 0, 0, 0};

    float value = 50;
    float min = 0;
    float max = 100;

    int background_color = 0x39C7;
    int border_color = TFT_WHITE;

    const TFT_eProgressBar_ValueStyle* value_style;

    TFT_eSprite* canvas = NULL;

    const int border_padding = 1;
};

void gui_pb_init(const TFT_eProgressBar& pb) 
{
    not_null(pb.canvas);

    pb.canvas->setColorDepth(16);
    pb.canvas->createSprite(pb.width, pb.height);
    pb.canvas->setSwapBytes(true);
    pb.canvas->fillSprite(pb.background_color);
}

void gui_pb_update(const TFT_eProgressBar& pb) 
{
    not_null(pb.canvas);

    auto left = border_with_padding(pb.borders_thickness[0], pb.border_padding);
    auto top = border_with_padding(pb.borders_thickness[1], pb.border_padding);
    auto right = border_with_padding(pb.borders_thickness[2], pb.border_padding);
    auto bottom = border_with_padding(pb.borders_thickness[3], pb.border_padding);

    auto w = pb.width - left - right;
    auto h = pb.height - top - bottom;

    pb.canvas->fillRect(left, top, w, h, pb.background_color);

    int value_w = map(pb.value, pb.min, pb.max, 0, w);

    pb.value_style->render(pb.canvas, left, top, w, h, value_w);
    
    pb.canvas->pushSprite(pb.left, pb.top);
}

void gui_pb_begin(const TFT_eProgressBar& pb) 
{
    not_null(pb.canvas);

    if (pb.borders_thickness[0] > 0) {
        pb.canvas->drawWideLine(0, 0, 0, 0 + pb.height, 
            pb.borders_thickness[0], 
            pb.border_color);
    }

    if (pb.borders_thickness[1] > 0) {
        pb.canvas->drawWideLine(0, 0, 0 + pb.width, 0, 
            pb.borders_thickness[1], 
            pb.border_color);
    }

    if (pb.borders_thickness[2] > 0) {
        pb.canvas->drawWideLine(0 + pb.width - 1, 0, 0 + pb.width - 1, 0 + pb.height, 
            pb.borders_thickness[2], 
            pb.border_color);
    }

    if (pb.borders_thickness[3] > 0) {
        pb.canvas->drawWideLine(0, 0 + pb.height - 1, 1 + pb.width, 0 + pb.height - 1, 
            pb.borders_thickness[3], 
            pb.border_color);
    }

    gui_pb_update(pb);
}