#pragma once

#include <TFT_eGUI.h>

#ifdef DEFAULT_FONT
  #define stringify(x) #x
  #define GEN_INC_PATH(a) stringify(a.h)
  #include GEN_INC_PATH(DEFAULT_FONT)
#endif

#ifdef RM67162_DRIVER
  #include "rm67162.h"
  TFT_eSprite lcd_buffer = TFT_eSprite(&tft);
#endif

TFT_eSpectrum<10> spectrum(&tft, 200, 150, 15, 15);

void gui_init()
{
#ifdef RM67162_DRIVER
  lcd_init();
  lcd_setRotation(1);
  lcd_brightness(200);

  lcd_buffer.createSprite(TFT_HEIGHT, TFT_WIDTH);
  lcd_buffer.fillSprite(TFT_BLACK);

  cassette.set_parent(&lcd_buffer);
#else
  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);
#endif

  spectrum.load_font(DEFAULT_FONT);
  spectrum.band_segment_padding = 5;

  spectrum.init();
  spectrum.begin();

  spectrum
    .set_value(0, 200)
    .set_value(9, 200)
    .set_value(5, 100);
}

float value = 0;

void gui_update()
{
    spectrum
        .set_value(3, value++)
        .update();

#ifdef RM67162_DRIVER
    lcd_PushColors(0, 0, TFT_HEIGHT, TFT_WIDTH, (uint16_t*)lcd_buffer.getPointer());
#endif
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