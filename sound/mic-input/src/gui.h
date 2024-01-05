#pragma once

#include <SPI.h>
#include <TFT_eSPI.h>

#include "Orbitron_Medium_20.h"
#include "Orbitron_Bold_12.h"
#include "NotoSansBold15.h"

#include "TFT_eGUI/TFT_eProgressBar.h"
#include "TFT_eGUI/TFT_eLed.h"
#include "TFT_eGUI/TFT_eScale.h"
#include "TFT_eGUI/TFT_ePanel.h"

TFT_eSPI tft = TFT_eSPI();

TFT_eSprite left_pb_canvas = TFT_eSprite(&tft); 
TFT_eSprite right_pb_canvas = TFT_eSprite(&tft); 

TFT_eSprite main_led_sprite = TFT_eSprite(&tft);
TFT_eSprite second_led_sprite = TFT_eSprite(&tft);

TFT_eSprite scale_sprite = TFT_eSprite(&tft);
TFT_eSprite panel_sprite = TFT_eSprite(&tft);

TFT_eProgressBar left_pb;
TFT_eProgressBar right_pb;

TFT_eLed main_led;
TFT_eLed second_led;

TFT_ePanel main_panel;

TFT_eScale scale(scale_sprite, {3, 1, 0, -1, -3, -5, -10, -20}, "dB");

const static TFT_eSolidBrush RedBrush(TFT_RED);
const static TFT_eSolidBrush DarkRedBrush(TFT_DARK_RED_8);

const static TFT_eSolidBrush GreenBrush(TFT_GREEN);
const static TFT_eSolidBrush DarkGreenBrush(TFT_DARKGREEN, 100);

const static TFT_eGradientBrush GreenGradientBrush(TFT_GREENYELLOW, TFT_GREEN, true);
const static TFT_eGradientBrush RedGradientBrush(TFT_RED, TFT_DARK_RED_12, true);

static TFT_eChevronBrush YellowChevronBrush(TFT_YELLOW, TFT_DARK_DARK_GRAY);

const static TFT_eProgressBar_SimpleValueStyle lime_gradient_pb_style(GreenGradientBrush);
const static TFT_eProgressBar_SimpleValueStyle red_gradient_pb_style(RedGradientBrush);
const static TFT_eProgressBar_SimpleValueStyle chevron_pb_style(YellowChevronBrush);
const static TFT_eProgressBar_SegmentedValueStyle lime_segmented_pb_style(GreenBrush, RedBrush, DarkGreenBrush, DarkRedBrush, 3, 16);

void gui_init() {
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

    left_pb.top = 10;
    left_pb.left = 15;
    left_pb.width = 205;
    left_pb.max = 1200;// 4095;

    left_pb.canvas = &left_pb_canvas;
    left_pb.value_style = &lime_segmented_pb_style;
    left_pb.background_color = TFT_BLACK;

    right_pb.top = 40;
    right_pb.left = 15;
    right_pb.width = 205;
    right_pb.max = 4095;

    right_pb.canvas = &right_pb_canvas;
    right_pb.value_style = &lime_segmented_pb_style;
    right_pb.background_color = TFT_BLACK;

    // right_pb.borders_thickness[0] = 1;
    // right_pb.borders_thickness[1] = 1;
    // right_pb.borders_thickness[2] = 1;
    // right_pb.borders_thickness[3] = 1; 

    scale.left = 0;
    scale.top = 65;
    scale.width = 230;
    scale.height = 25;
    scale.show_marks = true;
    scale.show_labels = false;
    scale.canvas = &scale_sprite;

    scale_sprite.loadFont(NotoSansBold15);

    main_panel.left = 0;
    main_panel.top = 100;
    main_panel.width = TFT_HEIGHT;
    main_panel.height = 20;
    main_panel.canvas = &panel_sprite;
    main_panel.background = &YellowChevronBrush;

    // gui_led_init(main_led);
    // gui_led_init(second_led);

    gui_pb_init(left_pb);
    gui_pb_init(right_pb);

    // gui_led_begin(main_led);
    // gui_led_begin(second_led);

    gui_pb_begin(left_pb);
    gui_pb_begin(right_pb);

    gui_scale_init(scale);
    gui_scale_begin(scale);

    gui_panel_init(main_panel);
    gui_panel_begin(main_panel);
}

long last_update_ms = millis();

void gui_analogread_task(void *arg)  
{
    while (1) 
    {
        auto left = analogRead(12);
        auto right = analogRead(13);

        //left_pb.value = left;
        right_pb.value = right;

        main_led.value = left > 2000;
        second_led.value = right > 500;

        gui_pb_update(left_pb);
        gui_pb_update(right_pb);

        if (last_update_ms - millis() > 100) {
            last_update_ms = millis();

            if (YellowChevronBrush.left > 200) {
                YellowChevronBrush.left = -10;
            } else{
                YellowChevronBrush.left--;
            }
            gui_panel_update(main_panel);
        } 

        // gui_led_update(main_led);
        // gui_led_update(second_led);

        vTaskDelay(100 / portTICK_RATE_MS);
    }
}