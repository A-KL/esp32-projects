#pragma once

#include <TFT_eGUI.h>

#include "espressif_logo.h"

#ifdef DEFAULT_FONT
  #define stringify(x) #x
  #define GEN_INC_PATH(a) stringify(a.h)
  #include GEN_INC_PATH(DEFAULT_FONT)
#endif

const static TFT_eProgressBar_SegmentedValueStyle lime_segmented_pb_style(&GreenBrush, &RedBrush, &DarkGreenBrush, &DarkRedBrush, 3, 32);

static TFT_eProgressBar left_pb(&tft, &lime_segmented_pb_style, 10, 15, tft.width() - 15);
static TFT_eProgressBar right_pb(&tft, &lime_segmented_pb_style, 100, 15, tft.width() - 15);
static TFT_eSpectrum<15> spectrum(&tft, 250, 150, 0, 0);

void gui_init()
{
  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_WHITE);
  tft.drawBitmap(0, 0, (const uint8_t *)espressif_logo_featured, TFT_WIDTH, TFT_HEIGHT, TFT_WHITE);

  left_pb.init();
  right_pb.init();

  left_pb.begin();
  right_pb.begin();

  //spectrum.load_font(DEFAULT_FONT);
  // spectrum.band_segment_padding = 2;
  // spectrum.bar_style = &lime_segmented_style;

  // spectrum.init();
  // spectrum.begin();

  // spectrum
  //   .set_value(0, 200)
  //   .set_value(9, 200)
  //   .set_value(5, 100);
}

float value = 0;

void gui_update()
{
    // spectrum
    //    // .set_value(3, value++)
    //     .update();
}

void gui_update_task(void *arg)  
{
    while (1) 
    {
        gui_update();
        vTaskDelay(120 / portTICK_RATE_MS);
    }
}

void gui_run(int core) 
{
    xTaskCreate(gui_update_task, "gui_run", 2048, NULL, core, NULL);
}