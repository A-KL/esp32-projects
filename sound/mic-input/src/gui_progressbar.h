#pragma once

#include <TFT_eSPI.h> 

typedef struct {
    int top;
    int left;
    int width;
    int height;
} rect_t;

rect_t resize(const rect_t& rect, int left, int top, int right, int bottom) {
    return { 
        .top = rect.top + top, 
        .left = rect.left + left, 
        .width = rect.width - left - right, 
        .height = rect.height - top - bottom
    };
}

void fillRect(TFT_eSprite& sprite, const rect_t& rect, int color) {
    sprite.fillRect(rect.left, rect.top, rect.width, rect.height, color);
}

class progressbar_value_style_t {
    public:
        virtual void render(TFT_eSprite& sprite, int left, int top, int w, int h, int value_w) = 0;
};

class progressbar_value_style_gradient_t : public progressbar_value_style_t {
    public:
        progressbar_value_style_gradient_t(int color_from, int color_to, bool sharp = true) 
        : _color_from(color_from), _color_to(color_to), _sharp(sharp)
        { } 

        virtual void render(TFT_eSprite& sprite, int left, int top, int w, int h, int value_w) {

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

const progressbar_value_style_gradient_t pb_value_lime_gradient(TFT_GREENYELLOW, TFT_GREEN, true);

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

    int value_normal_color = TFT_GREEN;
    int value_normal_color_from = TFT_GREENYELLOW;

    int value_hight_from = 90;
    int value_hight_color = TFT_RED;
    int value_hight_color_from = TFT_ORANGE;

} gui_progressbar_t;

const int gui_progressbar_border_padding = 1;

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

    auto value_w = map(progressbar.value, progressbar.min, progressbar.max, 0, w);

    if (progressbar.value_normal_color == progressbar.value_normal_color_from) {
        sprite.fillRect(left, top, value_w, h, progressbar.value_normal_color);
    }
    else {
        sprite.fillRect(left, top, value_w, h/2, progressbar.value_normal_color_from);
        sprite.fillRect(left, top + h/2, value_w, h/2, progressbar.value_normal_color);
    }
    
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