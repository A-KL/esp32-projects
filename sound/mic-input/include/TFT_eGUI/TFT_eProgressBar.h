#pragma once

#include <map>
#include <vector>

#include <TFT_eSPI.h>

#include "TFT_eSPI_Ex.h"
#include "TFT_eColorBrush.h"

// struct TFT_eProgressBar_ValueThreshold
// {
//     TFT_eProgressBar_ValueThreshold(const TFT_eColorBrush* color_brush, int color_start) 
//         : color(color_brush), start(color_start)
//     { }

//     TFT_eProgressBar_ValueThreshold(const TFT_eColorBrush& color_brush, int color_start) 
//         : TFT_eProgressBar_ValueThreshold(&color_brush, color_start)
//     { }

//     const TFT_eColorBrush* color = NULL;
//     const int start = 0;
// };

class visual_style_t {
    public:
        virtual void render(TFT_eSprite* sprite, int left, int top, int w, int h, int value_w) const = 0;
};

class TFT_eProgressBar_SimpleStyle : public visual_style_t 
{
    public:
        TFT_eProgressBar_SimpleStyle(const TFT_eColorBrush* value_color) 
            : _value_color(value_color)
            { } 

        TFT_eProgressBar_SimpleStyle(const TFT_eColorBrush& color_brush) 
            : TFT_eProgressBar_SimpleStyle(&color_brush)
            { } 

        virtual void render(TFT_eSprite* sprite, int left, int top, int w, int h, int value_w) const 
        {
            _value_color->fillRect(sprite, left, top, value_w, h);
        };

    private:      
        const TFT_eColorBrush* _value_color;
};

const static TFT_eSolidBrush RedColor(TFT_RED);

class progressbar_segmented_style_t : public visual_style_t 
{
    public:
        progressbar_segmented_style_t(const TFT_eColorBrush* segment_color, const TFT_eColorBrush* segment_bg_color, int padding, int segments_count)
          : _segment_color(segment_color), 
            _segment_bg_color(segment_bg_color),
            _segment_padding(padding),
            _segments_count(segments_count),
            _thresholds(
                { 
                    { 0, segment_color }, 
                    { (segments_count * 0.9), &RedColor }
                })
        { }

        progressbar_segmented_style_t(const TFT_eColorBrush& segment_color, const TFT_eColorBrush& segment_bg_color, int padding, int segments_count)
            : progressbar_segmented_style_t(&segment_color, &segment_bg_color, padding, segments_count)
        { } 

        virtual void render(TFT_eSprite* sprite, int left, int top, int w, int h, int value_w) const {
            auto segment_w = w / _segments_count - _segment_padding;
            auto value_segments_count = value_w / segment_w;

            auto value_color = _thresholds.begin()->second;

            for (auto i=0; i<value_segments_count; i++) {
                auto segment_left = left + i * (segment_w + _segment_padding);
                //_segment_color->fillRect(sprite, segment_left, top, segment_w, h);
                value_color->fillRect(sprite, segment_left, top, segment_w, h);

                auto results = _thresholds.find(i);
                if (results != _thresholds.end()) {
                    value_color = results->second;
                }
            }

            for (auto i=value_segments_count; i<_segments_count + 1; i++) {
                auto segment_left = left + i * (segment_w + _segment_padding);
                _segment_bg_color->fillRect(sprite, segment_left, top, segment_w, h);
            }
        };

    private:   
        const TFT_eColorBrush* _segment_color;
        const TFT_eColorBrush* _segment_bg_color;
        
        const int _segment_padding;
        const int _segments_count;

        std::map<int, const TFT_eColorBrush*> _thresholds;
};

const int pb_border_padding = 1;

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

    const visual_style_t* value_style;

    TFT_eSprite* canvas = NULL;
};

inline int border_with_padding(int border, int padding = pb_border_padding) 
{
    return border + (border > 0 ? padding : 0);
}

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

    auto left = border_with_padding(pb.borders_thickness[0]);
    auto top = border_with_padding(pb.borders_thickness[1]);
    auto right = border_with_padding(pb.borders_thickness[2]);
    auto bottom = border_with_padding(pb.borders_thickness[3]);

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