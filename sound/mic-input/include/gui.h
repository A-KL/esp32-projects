#pragma once

#include <LovyanGFX.h>
#include <LGFX_AUTODETECT.hpp>
#include <TFT_eSprite_GFX.h>

using TFT_eSPI = LGFX;
using TFT_eSprite = TFT_eSprite_GFX;

#if defined ( SDL_h_ )
    static TFT_eSPI tft ( 320, 240, 2 );
#else
    static TFT_eSPI tft;
#endif

#if (TFT_HEIGHT > 320)
    #include "Orbitron_Bold_12.h"
#else
    #include "NotoSansBold15.h"
#endif

#include "NotoSansMonoSCB20.h"

#include <TFT_eGUI.h>

const static TFT_eGradientBrush GreenGradientBrush(TFT_GREENYELLOW, TFT_GREEN, true);
const static TFT_eGradientBrush RedGradientBrush(TFT_RED, TFT_DARK_RED_12, true);
const static TFT_eChevronBrush YellowChevronBrush(TFT_YELLOW, TFT_DARK_DARK_GRAY);

const static TFT_eProgressBar_SimpleValueStyle lime_gradient_pb_style(GreenGradientBrush);
const static TFT_eProgressBar_SimpleValueStyle red_gradient_pb_style(RedGradientBrush);
const static TFT_eProgressBar_SimpleValueStyle chevron_pb_style(YellowChevronBrush);
const static TFT_eProgressBar_SegmentedValueStyle lime_segmented_pb_style(&GreenBrush, &RedBrush, &DarkGreenBrush, &DarkRedBrush, 3, 32);

const static TFT_eProgressBar_SegmentedValueStyle lime_segmented_vertical_style(
    { {0, &YellowBrush}, {1, &GreenBrush}, {10, &YellowBrush}, {11, &GreenBrush}},
    { {0, &DarkGreenBrush} },
    3, 
    60);

TFT_eLed main_led(&tft);
TFT_eLed second_led(&tft);

TFT_ePanel main_panel(&tft, &YellowChevronBrush, 0, 100, TFT_HEIGHT, 20);

TFT_eLabel adc_label(&tft, "ADC", 4, TFT_DARK_DARK_GRAY);
TFT_eLabel i2s_label(&tft, "I2S", 4, TFT_GREEN);
TFT_eLabel disabled_label(&tft, "OPT", 4, TFT_DARK_DARK_GRAY);
TFT_eLabel ovr_label(&tft, "OVR", 4, TFT_DARK_DARK_GRAY);

TFT_eProgressBar left_pb(&tft, &lime_segmented_pb_style, TFT_HEIGHT - 10, 20, 5, 10);
TFT_eScale scale(&tft, {3, 1, 0, -1, -3, -5, -10, -20}, TFT_HEIGHT, 65, 0, 35);
TFT_eProgressBar right_pb(&tft, &lime_segmented_pb_style, TFT_HEIGHT - 10, 20, 110, 10);

void gui_set_input(int input)
{
    adc_label.foreground_color = TFT_DARK_DARK_GRAY;
    i2s_label.foreground_color = TFT_DARK_DARK_GRAY;
    disabled_label.foreground_color = TFT_DARK_DARK_GRAY;

    switch (input)
    {
    case 0:
        adc_label.foreground_color = TFT_CYAN;
        break;
     case 1:
        i2s_label.foreground_color = TFT_GREEN;
        break;
    case 2:
        disabled_label.foreground_color = TFT_YELLOW;
        break;
    }

    adc_label.update();
    i2s_label.update();
    disabled_label.update();
}

void gui_led_init() 
{
    main_led.top = 130;
    main_led.left = 15;
    main_led.padding = 4;
    main_led.checked = true;

    second_led.top = 130;
    second_led.left = 40;
    second_led.padding = 4;
    second_led.checked = false;
    second_led.round = false;
    second_led.on_color = TFT_RED;
    second_led.on_color_to = TFT_DARK_RED_12;
    second_led.off_color = TFT_DARK_RED_8;

    main_led.init();
    second_led.init();

    main_led.begin();
    second_led.begin();
}

void gui_notify_init() 
{
    main_panel.init();
    main_panel.begin();
}

void gui_meter_init() 
{
    // Left progress bar
    left_pb.max = 1200;
    left_pb.background_color = TFT_BLACK;

    // Scale
    scale.load_font(NotoSansBold15);
    scale.interval_layout = Both;
    scale.show_labels = true;
    scale.horizontal_labels = false;

    // Right progress bar
    right_pb.max = 1200;
    right_pb.background_color = TFT_BLACK;

    // right_pb.borders_thickness[0] = 1;
    // right_pb.borders_thickness[1] = 1;
    // right_pb.borders_thickness[2] = 1;
    // right_pb.borders_thickness[3] = 1; 

    left_pb.init();
    right_pb.init();

    left_pb.begin();
    right_pb.begin();

    scale.init();
    scale.begin();
}

void gui_labels_init() 
{
    auto top = TFT_HEIGHT - 40;

    adc_label.left = 15;
    adc_label.top = top;

    adc_label.load_font(NotoSansMonoSCB20);
    adc_label.init();
    adc_label.begin();

    i2s_label.left = adc_label.left + adc_label.width + 5;
    i2s_label.top = top;

    i2s_label.load_font(NotoSansMonoSCB20);
    i2s_label.init();
    i2s_label.begin();

    disabled_label.left = i2s_label.left + i2s_label.width + 5;
    disabled_label.top = top;

    disabled_label.load_font(NotoSansMonoSCB20);
    disabled_label.init();
    disabled_label.begin();

    ovr_label.left = disabled_label.left + disabled_label.width + 5;
    ovr_label.top = top;

    ovr_label.load_font(NotoSansMonoSCB20);
    ovr_label.init();
    ovr_label.begin();
}

void gui_progress_bars_update()
{
    ovr_label.foreground_color = right_pb.value > 1000 ? TFT_RED : TFT_DARK_DARK_GRAY;

    left_pb.update();
    right_pb.update();

    ovr_label.update();

    // if (last_update_ms - millis() > 100) {
    //     last_update_ms = millis();

    //     if (YellowChevronBrush.left > 200) {
    //         YellowChevronBrush.left = -10;
    //     } else{
    //         YellowChevronBrush.left--;
    //     }
    //     gui_panel_update(main_panel);
    // } 
}

void gui_init() 
{
    tft.init();
    tft.setRotation(TFT_ROTATE);
    tft.setSwapBytes(true);
    tft.fillScreen(TFT_BLACK);

    gui_meter_init();
    gui_labels_init();

    //gui_notify_init();
    //gui_init_spectrum();
}

void gui_update()
{
    // if (gui_cpu_get_cores() > 1) {
    //     return;
    // }
    gui_progress_bars_update();
}