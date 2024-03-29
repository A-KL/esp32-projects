#pragma once

#include <vector>

#include <TFT_eSPI.h> 

#include "TFT_eSPI_Ex.h"

struct TFT_eLabel
{
    int left = 0;
    int top = 0;

    int width = -1;
    int height = -1;

    int foreground_color = TFT_DARKGREY;
    int background_color = TFT_BLACK;

    const int default_padding_w = 2;
    const int default_padding_h = 2;

    const char* text;

    int borders_thickness[4] = {0, 0, 0, 0};

    TFT_eSprite* canvas = NULL;

    TFT_eLabel(TFT_eSprite& canvas, const char* label_text, int border = 0, int color = TFT_WHITE, int bg_color = TFT_BLACK) 
        : canvas(&canvas), text(label_text), foreground_color(color), background_color(bg_color), borders_thickness{border, border, border, border}
    { }
};

void gui_label_init(TFT_eLabel& label) 
{   
    not_null(label.canvas);

    label.height = label.canvas->fontHeight();
    label.height += label.borders_thickness[1];
    label.height += label.borders_thickness[3];
    label.height += label.default_padding_h;

    label.width = label.canvas->textWidth(label.text);
    label.width += label.borders_thickness[0];
    label.width += label.borders_thickness[2];
    label.width += label.default_padding_w;

    label.canvas->deleteSprite();
    label.canvas->setColorDepth(16);
    label.canvas->createSprite(label.width, label.height);
    label.canvas->setSwapBytes(true);
}

void gui_label_update(const TFT_eLabel& label) 
{
    not_null(label.canvas);

    label.canvas->fillSprite(label.background_color);

    if (label.borders_thickness[0] > 0) {
        label.canvas->drawWideLine(0, 0, 0, 0 + label.height, 
            label.borders_thickness[0], 
            label.foreground_color);
    }

    if (label.borders_thickness[1] > 0) {
        label.canvas->drawWideLine(0, 0, 0 + label.width, 0, 
            label.borders_thickness[1], 
            label.foreground_color);
    }

    if (label.borders_thickness[2] > 0) {
        label.canvas->drawWideLine(0 + label.width - 1, 0, 0 + label.width - 1, 0 + label.height, 
            label.borders_thickness[2], 
            label.foreground_color);
    }

    if (label.borders_thickness[3] > 0) {
        label.canvas->drawWideLine(0, 0 + label.height - 1, 1 + label.width, 0 + label.height - 1, 
            label.borders_thickness[3], 
            label.foreground_color);
    }

    auto text_string = String(label.text);

    label.canvas->setTextColor(label.foreground_color);
    label.canvas->drawCentreString(text_string, label.width/2, label.default_padding_h + label.borders_thickness[1], 1);
    
    label.canvas->pushSprite(label.left, label.top);
}

void gui_label_begin(const TFT_eLabel& label) 
{
    // not_null(label.canvas);

    // if (label.borders_thickness[0] > 0) {
    //     label.canvas->drawWideLine(0, 0, 0, 0 + label.height, 
    //         label.borders_thickness[0], 
    //         label.foreground_color);
    // }

    // if (label.borders_thickness[1] > 0) {
    //     label.canvas->drawWideLine(0, 10, 0 + label.width, 10, 
    //         label.borders_thickness[1], 
    //         label.foreground_color);
    // }

    // if (label.borders_thickness[2] > 0) {
    //     label.canvas->drawWideLine(0 + label.width - 1, 0, 0 + label.width - 1, 0 + label.height, 
    //         label.borders_thickness[2], 
    //         label.foreground_color);
    // }

    // if (label.borders_thickness[3] > 0) {
    //     label.canvas->drawWideLine(0, 0 + label.height - 1, 1 + label.width, 0 + label.height - 1, 
    //         label.borders_thickness[3], 
    //         label.foreground_color);
    // }

    gui_label_update(label);
}