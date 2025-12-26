#pragma once

#include <hal_arduino.h>
#include <Lcd.h>
#include <TFT_eGUI.h>

#ifdef DEFAULT_FONT
  #define stringify(x) #x
  #define GEN_INC_PATH(a) stringify(a.h)
  #include GEN_INC_PATH(DEFAULT_FONT)
#endif

const static TFT_eChevronBrush YellowChevronBrush(TFT_YELLOW, TFT_DARK_DARK_GRAY);
const static TFT_eProgressBar_SimpleValueStyle spectrum_style(YellowChevronBrush);
const static TFT_eProgressBar_SegmentedValueStyle lime_segmented_style(&GreenBrush, &RedBrush, &DarkGreenBrush, &DarkRedBrush, 2, 16);

TFT_eSpectrum<15> spectrum(&tft, 250, 150, 0, 0);

// float value = 0;

void gui_init()
{
  tft.init();
  tft.setRotation(TFT_ROTATE);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);

  //spectrum.load_font(DEFAULT_FONT);
  spectrum.band_segment_padding = 2;
  //spectrum.bar_style = &lime_segmented_style;

  spectrum.init();
  spectrum.begin();

  // spectrum
  //   .set_value(0, 200)
  //   .set_value(9, 200)
  //   .set_value(5, 100);
}

void gui_update()
{
  spectrum
      // .set_value(3, value++)
      .update();
}

void gui_update_task(void *arg)  
{
    while (1) 
    {
        gui_update();
        delay(50);
    }
}

void gui_run() 
{
  //  xTaskCreate(gui_update_task, "gui_run", 2048, NULL, core, NULL);
}