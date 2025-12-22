#pragma once

#include <Lcd.h>
#include "TFT_eGUI.h"

#if (TFT_HEIGHT > 320)
    #include "Orbitron_Bold_12.h"
#else
    #include "NotoSansBold15.h"
#endif

#include "NotoSansMonoSCB20.h"

const static TFT_eGradientBrush GreenGradientBrush(TFT_GREENYELLOW, TFT_GREEN, true);
const static TFT_eGradientBrush RedGradientBrush(TFT_RED, TFT_DARK_RED_12, true);

// const static TFT_eChevronBrush YellowChevronBrush(TFT_YELLOW, TFT_DARK_DARK_GRAY);

const static TFT_eProgressBar_SimpleValueStyle lime_gradient_pb_style(GreenGradientBrush);
const static TFT_eProgressBar_SimpleValueStyle red_gradient_pb_style(RedGradientBrush);

const static TFT_eProgressBar_SimpleValueStyle green_style(DarkGreenBrush);
// const static TFT_eProgressBar_SimpleValueStyle chevron_pb_style(YellowChevronBrush);

const static TFT_eProgressBar_SegmentedValueStyle lime_segmented_pb_style(
    &GreenBrush, &RedBrush, &DarkGreenBrush, &DarkRedBrush, 3, 32);

const static TFT_eProgressBar_SegmentedValueStyle lime_segmented_sp_style(
    &GreenBrush, &RedBrush, &DarkGreenBrush, &DarkRedBrush, 2, 15);

// const static TFT_eProgressBar_SegmentedValueStyle white_segmented_pb_style(
//     &WhiteBrush, &RedBrush, &DarkGreenBrush, &DarkWhiteBrush, 2, 46);

// const static TFT_eProgressBar_SegmentedValueStyle lime_segmented_vertical_style(
//     { {0, &YellowBrush}, {1, &GreenBrush}, {10, &YellowBrush}, {11, &GreenBrush}},
//     { {0, &DarkGreenBrush} },
//     3, 
//     60);

TFT_eSpectrum<20> spectrum(&tft, 300, 120, 10, 10);
TFT_eLabel spectrum_label(&tft, "20    65   125   250   500  1K  2K  4K  8K  16K", 0, TFT_DARKGREY, 0, 130, 320, 20);

TFT_eLabel sample_size_label(&tft, "32bit", 0, TFT_DARK_DARK_GRAY, 10, 160, 95, 25);
TFT_eLabel sample_res_label(&tft, "48K", 0, TFT_DARK_DARK_GRAY, 110, 160, 95, 25);
TFT_eLabel input_label(&tft, "COAX", 0, TFT_DARK_DARK_GRAY, 210, 160, 95, 25);

TFT_eLabel l_label(&tft, "L", 0, TFT_DARKGREY, 0, 195, 20, 15);
TFT_eLabel r_label(&tft, "R", 0, TFT_DARKGREY, 0, 215, 20, 15);
TFT_eProgressBar left_pb(&tft, &lime_segmented_pb_style, 20, 195, 290, 15);
TFT_eProgressBar right_pb(&tft, &lime_segmented_pb_style, 20, 215, 290, 15);

TFT_eScale scale(&tft, {3, 1, 0, -1, -3, -5, -10, -20}, tft.height(), 60, 0, 35);

int gui_cpu_get_cores() 
{
    esp_chip_info_t info;
    esp_chip_info(&info);
    
    return info.cores;
}

void gui_meter_init() {
    // Left progress bar
   left_pb.max = INT16_MAX * 200;// 1200;
   left_pb.background_color = TFT_BLACK;

    // Scale
    scale.load_font(NotoSansBold15);
    scale.interval_layout = Both;
    scale.show_labels = true;
    scale.horizontal_labels = false;

    // Right progress bar
    right_pb.max =  INT16_MAX * 200;//1200;
    right_pb.background_color = TFT_BLACK;

    // right_pb.borders_thickness[0] = 1;
    // right_pb.borders_thickness[1] = 1;
    // right_pb.borders_thickness[2] = 1;
    // right_pb.borders_thickness[3] = 1; 

    l_label.load_font(NotoSansBold15);
    r_label.load_font(NotoSansBold15);

    left_pb.init();
    right_pb.init();

    left_pb.begin();
    right_pb.begin();

    l_label.init();
    r_label.init();

    l_label.begin();
    r_label.begin();

    // scale.init();
    // scale.begin();
}

void gui_labels_init() 
{
    sample_size_label.load_font(NotoSansMonoSCB20);
    sample_size_label.background_color = TFT_DARK_DARK_GRAY;
    sample_size_label.foreground_color = TFT_WHITE;
    sample_size_label.init();
    sample_size_label.begin();

    sample_res_label.load_font(NotoSansMonoSCB20);
    sample_res_label.background_color = TFT_DARK_DARK_GRAY;
    sample_res_label.foreground_color = TFT_RED; // TFT_WHITE
    sample_res_label.init();
    sample_res_label.begin();

    input_label.load_font(NotoSansMonoSCB20);
    input_label.background_color = TFT_DARK_DARK_GRAY;
    input_label.foreground_color = TFT_WHITE;
    input_label.init();
    input_label.begin();

    spectrum_label.load_font(NotoSansBold15);
   // spectrum_label.background_color = TFT_RED;
    spectrum_label.init();
    spectrum_label.begin();
}

void gui_spectrum_init()
{
  spectrum.background_color = TFT_BLACK;
  spectrum.bar_background_color = TFT_DARK_DARK_GRAY;
 // spectrum.bar_style = &green_style;
  // spectrum.bar_style = &green_style;

  srand(time(NULL));

  spectrum.init();
  spectrum.begin();

  for (auto i=0; i<spectrum.size(); i++) {
    spectrum.set_value(i, rand() % 255);
  }
}

void gui_init() 
{
    gui_spectrum_init();
    gui_labels_init();
    gui_meter_init();
}

void _gui_update()
{
    left_pb.update();
    right_pb.update();

    spectrum.update();

    sample_size_label.update();
    sample_res_label.update();
    input_label.update();

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

void gui_update()
{
    if (gui_cpu_get_cores() > 1) {
        return;
    }
    _gui_update();
}

void gui_update_task(void *arg)  
{
    while (1) 
    {
        _gui_update();
        vTaskDelay(100 / portTICK_RATE_MS);
    }
}

void gui_begin() 
{
    if (gui_cpu_get_cores() < 1) {
        return;
    }
    xTaskCreate(gui_update_task, "gui_run", 2048, NULL, 0, NULL);
}