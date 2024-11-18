#pragma once

#include <TFT_eSPI.h>

//#include "Orbitron_Bold_12.h"
#include "NotoSansBold15.h"
#include "NotoSansMonoSCB20.h"

#include "TFT_eGUI/TFT_eProgressBar.h"
#include "TFT_eGUI/TFT_eLed.h"
#include "TFT_eGUI/TFT_eScale.h"
#include "TFT_eGUI/TFT_ePanel.h"
#include "TFT_eGUI/TFT_eLabel.h"
#include "TFT_eGUI/TFT_eSpectrum.h"

TFT_eSPI tft = TFT_eSPI();

TFT_eSprite left_pb_canvas(&tft); 
TFT_eSprite right_pb_canvas(&tft); 

TFT_eSprite main_led_sprite(&tft);
TFT_eSprite second_led_sprite(&tft);

TFT_eSprite scale_sprite(&tft);
TFT_eSprite scale_text_sprite(&tft);
TFT_eSprite panel_sprite(&tft);

TFT_eSprite line_label_sprite(&tft);

TFT_eSprite spectrum_sprite(&tft);

TFT_eProgressBar left_pb;
TFT_eProgressBar right_pb;

TFT_eLed main_led;
TFT_eLed second_led;

TFT_ePanel main_panel;

TFT_eLabel adc_label(line_label_sprite, "ADC", 4, TFT_DARK_DARK_GRAY);
TFT_eLabel i2s_label(line_label_sprite, "I2S", 4, TFT_GREEN);
TFT_eLabel disabled_label(line_label_sprite, "OPT", 4, TFT_DARK_DARK_GRAY);
TFT_eLabel ovr_label(line_label_sprite, "OVR", 4, TFT_DARK_DARK_GRAY);

TFT_eScale scale(scale_sprite, scale_text_sprite, {3, 1, 0, -1, -3, -5, -10, -20}, "dB");

TFT_eSpectrum<10> spectrum(spectrum_sprite, 60);

const static TFT_eSolidBrush RedBrush(TFT_RED);
const static TFT_eSolidBrush DarkRedBrush(TFT_RED, 20);

const static TFT_eSolidBrush YellowBrush(TFT_YELLOW);
const static TFT_eSolidBrush GreenBrush(TFT_GREEN);
const static TFT_eSolidBrush DarkGreenBrush(TFT_DARKGREEN, 20);

const static TFT_eGradientBrush GreenGradientBrush(TFT_GREENYELLOW, TFT_GREEN, true);
const static TFT_eGradientBrush RedGradientBrush(TFT_RED, TFT_DARK_RED_12, true);

static TFT_eChevronBrush YellowChevronBrush(TFT_YELLOW, TFT_DARK_DARK_GRAY);

const static TFT_eProgressBar_SimpleValueStyle lime_gradient_pb_style(GreenGradientBrush);
const static TFT_eProgressBar_SimpleValueStyle red_gradient_pb_style(RedGradientBrush);
const static TFT_eProgressBar_SimpleValueStyle chevron_pb_style(YellowChevronBrush);
const static TFT_eProgressBar_SegmentedValueStyle lime_segmented_pb_style(&GreenBrush, &RedBrush, &DarkGreenBrush, &DarkRedBrush, 3, 32);

const static TFT_eProgressBar_SegmentedValueStyle lime_segmented_vertical_style(
    { {0, &YellowBrush}, {1, &GreenBrush}, {10, &YellowBrush}, {11, &GreenBrush}},
    { {0, &DarkGreenBrush} },
    3, 
    60);

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

    gui_label_update(adc_label);
    gui_label_update(i2s_label);
    gui_label_update(disabled_label);
}

void gui_led_init() {
    main_led.top = 100;
    main_led.left = 15;
    main_led.value = true;
    main_led.canvas = &main_led_sprite;

    second_led.top = 100;
    second_led.left = 40;
    second_led.value = true;
    second_led.canvas = &second_led_sprite;
    second_led.on_color = TFT_RED;
    second_led.on_color_to = TFT_DARK_RED_12;
    second_led.off_color = TFT_DARK_RED_8;

    gui_led_init(main_led);
    gui_led_init(second_led);

    gui_led_begin(main_led);
    gui_led_begin(second_led);
}

void gui_notify_init() {
    main_panel.left = 0;
    main_panel.top = 100;
    main_panel.width = TFT_HEIGHT;
    main_panel.height = 20;
    main_panel.canvas = &panel_sprite;
    main_panel.background = &YellowChevronBrush;

    gui_panel_init(main_panel);
    gui_panel_begin(main_panel);
}

void gui_meter_init() {
    // Left progress bar
    left_pb.top = 10;
    left_pb.left = 15;
    left_pb.width = tft.width() - left_pb.left;
    left_pb.max = 1200;

    left_pb.canvas = &left_pb_canvas;
    left_pb.value_style = &lime_segmented_pb_style;
    left_pb.background_color = TFT_BLACK;

    // Scale
    scale.left = 0;
    scale.top = 35;
    scale.width = tft.width();
    scale.height = 60;
    scale.interval_layout = Both;
    scale.show_labels = true;
    scale.horizontal_labels = false;

    scale_sprite.loadFont(NotoSansBold15);
    scale_text_sprite.loadFont(NotoSansBold15);

    // Right progress bar
    right_pb.top = 100;
    right_pb.left = 15;
    right_pb.width = tft.width() - right_pb.left;
    right_pb.max = 1200;

    right_pb.canvas = &right_pb_canvas;
    right_pb.value_style = &lime_segmented_pb_style;
    right_pb.background_color = TFT_BLACK;

    // right_pb.borders_thickness[0] = 1;
    // right_pb.borders_thickness[1] = 1;
    // right_pb.borders_thickness[2] = 1;
    // right_pb.borders_thickness[3] = 1; 

    gui_pb_init(left_pb);
    gui_pb_init(right_pb);

    gui_pb_begin(left_pb);
    gui_pb_begin(right_pb);

    gui_scale_init(scale);
    gui_scale_begin(scale);
}

void gui_labels_init() {
    line_label_sprite.loadFont(NotoSansMonoSCB20);
    //ovr_label_sprite.loadFont(NotoSansMonoSCB20);

    auto top = TFT_HEIGHT - 40;

    adc_label.left = 15;
    adc_label.top = top;

    gui_label_init(adc_label);
    gui_label_begin(adc_label);

    i2s_label.left = adc_label.left + adc_label.width + 5;
    i2s_label.top = top;

    gui_label_init(i2s_label);
    gui_label_begin(i2s_label);

    disabled_label.left = i2s_label.left + i2s_label.width + 5;
    disabled_label.top = top;

    gui_label_init(disabled_label);
    gui_label_begin(disabled_label);

    ovr_label.left = disabled_label.left + disabled_label.width + 5;
    ovr_label.top = top;

    gui_label_init(ovr_label);
    gui_label_begin(ovr_label);
}

void gui_init_spectrum() {
    spectrum.width = TFT_WIDTH;
    spectrum.height = TFT_HEIGHT;
    spectrum.values[0] = 100;
    spectrum.values[1] = 100;
    spectrum.values[2] = 100;
    spectrum.values[3] = 100;
    spectrum.values[4] = 50;
    spectrum.values[5] = spectrum.max / 2;
    spectrum.values[7] = spectrum.max / 3;
    spectrum.values[8] = spectrum.max / 3;
    spectrum.values[9] = spectrum.max;

    spectrum.init();
    spectrum.begin();

    // gui_spectrum_init(spectrum);
    // gui_spectrum_begin(spectrum);
}

void gui_init() 
{
    gui_meter_init();
    gui_labels_init();

    //gui_notify_init();
    //gui_init_spectrum();
}

void gui_progress_bars_update()
{
    ovr_label.foreground_color = right_pb.value > 1000 ? TFT_RED : TFT_DARK_DARK_GRAY;

    gui_pb_update(left_pb);
    gui_pb_update(right_pb);

    gui_label_update(ovr_label);

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

void gui_update_task(void *arg)  
{
    while (1) 
    {
        gui_progress_bars_update();
        vTaskDelay(100 / portTICK_RATE_MS);
    }
}

void gui_run(int core) 
{
    xTaskCreate(gui_update_task, "gui_run", 2048, NULL, core, NULL);
}