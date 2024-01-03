#pragma once

#include <TFT_eSPI.h> 

#include "TFT_eSPI_Ex.h"
#include "TFT_eColorBrush.h"

struct TFT_ePanel
{
    int left = 0;
    int top = 0;
    int width = 40;
    int height = 20;

    const TFT_eColorBrush* background = NULL;

    TFT_eSprite* canvas = NULL;
};

void gui_panel_init(const TFT_ePanel& panel) 
{
    panel.canvas->setColorDepth(16);
    panel.canvas->createSprite(panel.width, panel.height);
    panel.canvas->setSwapBytes(true);
}

void gui_panel_update(const TFT_ePanel& panel) 
{ 
    panel.background->fillRect(panel.canvas, 0, 0, panel.width, panel.height);

    panel.canvas->pushSprite(panel.left, panel.top);
}

void gui_panel_begin(const TFT_ePanel& led) 
{
    gui_panel_update(led);
}