#pragma once

#include <TFT_eSPI.h> 
#include <TFT_eSPI_Ex.h> 

class visual_style_t {
    public:
        virtual void render(TFT_eSprite& sprite, int left, int top, int w, int h, int value_w) const = 0;
};

class progressbar_gradient_style_t : public visual_style_t {
    public:
        progressbar_gradient_style_t(int color_from, int color_to, bool sharp = true) 
            : _color_from(color_from), _color_to(color_to), _sharp(sharp)
            { } 

        virtual void render(TFT_eSprite& sprite, int left, int top, int w, int h, int value_w) const {

            if (_sharp) {
                sprite.fillRect(left, top, value_w, h/2, _color_from);
                sprite.fillRect(left, top + h/2, value_w, h/2, _color_to);
            }
            else {
                sprite.fillRectVGradient(left, top, value_w, h, _color_from, _color_to);
            }
        };

    private:      
        const int _color_from;
        const int _color_to;
        const bool _sharp;
};

class progressbar_segmented_style_t : public visual_style_t {
    public:
        progressbar_segmented_style_t(int padding = 2, int segments_count = 10, int segment_color = TFT_GREEN, int segment_bg_color = TFT_DARKGREEN) 
            : _segment_color(segment_color), _segment_padding(padding), _segments_count(segments_count), _segment_bg_color(segment_bg_color)
            { } 

        virtual void render(TFT_eSprite& sprite, int left, int top, int w, int h, int value_w) const {
            auto segment_w = w / _segments_count - _segment_padding;
            auto value_segments_count = value_w / segment_w;

            for (auto i=0; i<value_segments_count; i++) {
                auto segment_left = left + i * (segment_w + _segment_padding);
                sprite.fillRect(segment_left, top, segment_w, h, _segment_color);
            }

            for (auto i=value_segments_count; i<_segments_count + 1; i++) {
                auto segment_left = left + i * (segment_w + _segment_padding);
                sprite.fillRect(segment_left, top, segment_w, h, _segment_bg_color);
            }
        };

    private:      
        const int _segment_color;
        const int _segment_bg_color;
        const int _segment_padding;
        const int _segments_count;
};

const int gui_progressbar_border_padding = 1;

const progressbar_gradient_style_t pb_lime_gradient_style(TFT_GREENYELLOW, TFT_GREEN, true);
const progressbar_gradient_style_t red_gradient_pb_style(TFT_RED, TFT_DARK_RED, true);
const progressbar_segmented_style_t pb_lime_segmented_style(3, 16, TFT_GREEN, TFT_DARKGREEN);

typedef struct {
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

    const visual_style_t* value_style = &pb_lime_gradient_style;

} gui_progressbar_t;

void gui_progressbar_init(TFT_eSprite& sprite, const gui_progressbar_t& progressbar) {
    sprite.setColorDepth(16);
    sprite.createSprite(progressbar.width, progressbar.height);
    sprite.setSwapBytes(true);

    sprite.fillSprite(progressbar.background_color);
}

inline int border_with_padding(int border, int padding = gui_progressbar_border_padding) {
    return border + (border > 0 ? padding : 0);
}

void gui_progressbar_update(TFT_eSprite& sprite, const gui_progressbar_t& progressbar) {

    auto left = border_with_padding(progressbar.borders_thickness[0]);
    auto top = border_with_padding(progressbar.borders_thickness[1]);
    auto right = border_with_padding(progressbar.borders_thickness[2]);
    auto bottom = border_with_padding(progressbar.borders_thickness[3]);

    auto w = progressbar.width - left - right;
    auto h = progressbar.height - top - bottom;

    sprite.fillRect(left, top, w, h, progressbar.background_color);

    int value_w = map(progressbar.value, progressbar.min, progressbar.max, 0, w);

    progressbar.value_style->render(sprite, left, top, w, h, value_w);
    
    sprite.pushSprite(progressbar.left, progressbar.top);
}

void gui_progressbar_begin(TFT_eSprite& sprite, const gui_progressbar_t& progressbar) {

    if (progressbar.borders_thickness[0] > 0) {
        sprite.drawWideLine(0, 0, 0, 0 + progressbar.height, 
            progressbar.borders_thickness[0], 
            progressbar.border_color);
    }

    if (progressbar.borders_thickness[1] > 0) {
        sprite.drawWideLine(0, 0, 0 + progressbar.width, 0, 
            progressbar.borders_thickness[1], 
            progressbar.border_color);
    }

    if (progressbar.borders_thickness[2] > 0) {
        sprite.drawWideLine(0 + progressbar.width - 1, 0, 0 + progressbar.width - 1, 0 + progressbar.height, 
            progressbar.borders_thickness[2], 
            progressbar.border_color);
    }

    if (progressbar.borders_thickness[3] > 0) {
        sprite.drawWideLine(0, 0 + progressbar.height - 1, 1 + progressbar.width, 0 + progressbar.height - 1, 
            progressbar.borders_thickness[3], 
            progressbar.border_color);
    }

    gui_progressbar_update(sprite, progressbar);
}