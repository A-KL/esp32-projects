#pragma once

#include <TFT_eSPI.h>

#include "TFT_eProgressBar_ValueStyle.h"

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