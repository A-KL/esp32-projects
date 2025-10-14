#pragma once

#include "TFT_eSPI_Ex.h"

struct TFT_eLed 
{
    int left = 0;
    int top = 0;
    int width = 20;

    bool round = true;

    bool value = false;

    int on_color = TFT_GREENYELLOW;
    int on_color_to = TFT_GREEN;

    int off_color = TFT_DARKGREEN;

    int bg_color = TFT_WHITE;
    int bg_color_to = TFT_DARKGREY;

    TFT_eSprite* canvas = NULL;
};

void gui_led_init(const TFT_eLed& led) 
{
    led.canvas->setColorDepth(16);
    led.canvas->createSprite(led.width, led.width);
    led.canvas->setSwapBytes(true);
}

void gui_led_round_update(const TFT_eLed& led) 
{
    auto padding = led.width / 10;
    auto center = led.width / 2;

    if (padding <= 0)
        padding = 1;

    auto r = (led.width - padding) / 2 - 1;

    led.canvas->fillCircle(center, center, r, led.value ? led.on_color : led.off_color);
    led.canvas->pushSprite(led.left, led.top);
}

void gui_led_update(const TFT_eLed& led) 
{
    if (led.round) 
    {
        gui_led_round_update(led);
        return;
    }

    auto padding = led.width / 10;

    if (padding <= 0)
        padding = 1;

    auto led_size = led.width - padding * 2;

    if (led.bg_color == led.bg_color_to) 
    {
        led.canvas->fillRect(padding, padding, led_size, led_size, 
        led.value ? led.on_color : led.off_color);
    } 
    else 
    {
        led.canvas->fillRectVGradient(padding, padding, led_size, led_size, 
        led.value ? led.on_color : led.off_color,
        led.value ? led.on_color_to : led.off_color);
    }

    led.canvas->pushSprite(led.left, led.top);
}

void gui_led_begin(const TFT_eLed& led) 
{
    if (led.round) 
    {
        auto center = led.width / 2;
        led.canvas->fillSmoothCircle(center, center, led.width / 2 - 1, led.bg_color);
    } 
    else 
    {
        if (led.bg_color == led.bg_color_to)
            led.canvas->fillRect(0, 0, led.width, led.width, led.bg_color);
        else
            led.canvas->fillRectVGradient(0, 0, led.width, led.width, led.bg_color, led.bg_color_to);
    }
    gui_led_update(led);
}