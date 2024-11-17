#pragma once

#include <map>
#include <vector>
#include <TFT_eSPI.h>

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

class TFT_eProgressBar_SlimSegmentsValueStyle : public TFT_eProgressBar_ValueStyle 
{
    public:
        TFT_eProgressBar_SlimSegmentsValueStyle(
            const TFT_eColorBrush* segment_color,
            const TFT_eColorBrush* segment_bg_color,
            const int padding,
            const int segments_count)
            : _segment_color(segment_color), _segment_bg_color(segment_bg_color)
            { } 

        TFT_eProgressBar_SlimSegmentsValueStyle(
            const TFT_eColorBrush &segment_color,
            const TFT_eColorBrush &segment_bg_color,
            const int padding,
            const int segments_count)
            : TFT_eProgressBar_SlimSegmentsValueStyle(&segment_color, &segment_bg_color, padding, segments_count)
            { } 

        virtual void render(TFT_eSprite* sprite, int left, int top, int w, int h, int value_w) const 
        {
            if (w > h) {
                render_horizontal(sprite, left, top, w, h, value_w);
            } else {
                render_vertical(sprite, left, top, w, h, value_w);
            }
        };

    private:      
        const TFT_eColorBrush* _segment_color;
        const TFT_eColorBrush* _segment_bg_color;

        void render_horizontal(TFT_eSprite* sprite, int left, int top, int w, int h, int value_w) const 
        {
            
        }

        void render_vertical(TFT_eSprite* sprite, int left, int top, int w, int h, int value_h) const 
        {

        }
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
            :  TFT_eProgressBar_SegmentedValueStyle(
               { 
                    { 0, segment_color }, 
                    { (segments_count * 0.9), segment_hi_color }
                },
                { 
                    { 0, segment_bg_color }, 
                    { (segments_count * 0.9), segment_hi_bg_color }
                },
                padding,
                segments_count)
        { }

        TFT_eProgressBar_SegmentedValueStyle(
            const TFT_eColorBrush* foreground_color,
            const TFT_eColorBrush* background_color,
            const int padding, 
            const int segments_count)
            :  TFT_eProgressBar_SegmentedValueStyle(
                { {0, foreground_color} }, 
                { {0, background_color} }, 
                padding, 
                segments_count)
        { }

        TFT_eProgressBar_SegmentedValueStyle(
            const std::map<int, const TFT_eColorBrush*> &thresholds_foreground,
            const std::map<int, const TFT_eColorBrush*> &thresholds_background,
            const int padding, 
            const int segments_count)
                :  _segment_padding(padding),
                   _segments_count(segments_count),
                   _thresholds(thresholds_foreground), 
                   _bg_thresholds(thresholds_background)                   
        { } 

        virtual void render(TFT_eSprite* sprite, int left, int top, int w, int h, int value_w) const 
        {
            if (w > h) {
                render_horizontal(sprite, left, top, w, h, value_w);
            } else {
                render_vertical(sprite, left, top, w, h, value_w);
            }
        }

    private:
        const int _segment_padding;
        const int _segments_count;

        std::map<int, const TFT_eColorBrush*> _thresholds;
        std::map<int, const TFT_eColorBrush*> _bg_thresholds;

        void render_horizontal(TFT_eSprite* sprite, int left, int top, int w, int h, int value_w) const 
        {
            auto segment_w = (w - _segment_padding * (_segments_count - 1)) / _segments_count;
            auto value_segments_count = value_w / (segment_w + _segment_padding);

            auto value_color = _thresholds.begin()->second;
            auto background_color = _bg_thresholds.begin()->second;

            for (auto i=0; i < value_segments_count; i++) 
            {
                auto segment_left = left + i * (segment_w + _segment_padding);
                value_color->fillRect(sprite, segment_left, top, segment_w, h);

                auto results = _thresholds.find(i);
                if (results != _thresholds.end()) {
                    value_color = results->second;
                }
            }

            for (auto i=value_segments_count; i<_segments_count; i++)
            {
                auto segment_left = left + i * (segment_w + _segment_padding);
                background_color->fillRect(sprite, segment_left, top, segment_w, h);

                auto results = _bg_thresholds.find(i);
                if (results != _bg_thresholds.end()) {
                    background_color = results->second;
                }
            }
        };

        void render_vertical(TFT_eSprite* sprite, int left, int top, int w, int h, int value_h) const 
        {
            auto segment_h = (h - _segment_padding * (_segments_count - 1)) / _segments_count;
            auto value_segments_count = value_h / (segment_h + _segment_padding);

            auto value_color = _thresholds.begin()->second;
            auto background_color = _bg_thresholds.begin()->second;

            for (auto i=0; i < value_segments_count; i++) 
            {
                auto segment_top = h - top - i * (segment_h + _segment_padding);
                value_color->fillRect(sprite, left, segment_top, w, segment_h);

                auto results = _thresholds.find(i);
                if (results != _thresholds.end()) {
                    value_color = results->second;
                }
            }

            for (auto i=value_segments_count; i<_segments_count; i++)
            {
            auto segment_top = h - top - i * (segment_h + _segment_padding);
                background_color->fillRect(sprite, left, segment_top, w, segment_h);

                auto results = _bg_thresholds.find(i);
                if (results != _bg_thresholds.end()) {
                    background_color = results->second;
                }
            }
        };
};
