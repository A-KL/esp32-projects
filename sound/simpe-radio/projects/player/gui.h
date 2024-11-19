#pragma once

#include <TFT_eGUI.h>
#include "NotoSansBold15.h"

#ifdef RM67162_DRIVER
#include "rm67162.h"
TFT_eSprite lcd_buffer = TFT_eSprite(&tft);
#endif

TFT_eCassettePlayer player(&tft, TFT_HEIGHT, TFT_WIDTH, 0, 0);

void gui_init()
{
  tft.init();
  tft.setRotation(TFT_ROTATE);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);

#ifdef RM67162_DRIVER
  lcd_buffer.createSprite(TFT_WIDTH, TFT_HEIGHT);
  lcd_buffer.setSwapBytes(1);
  lcd_buffer.fillSprite(TFT_BLACK);
  player.set_parent(&lcd_buffer);

  lcd_init();
  lcd_brightness(200);
#endif

  // tft.setFreeFont(&Orbitron_Medium_20);
  player.load_font(NotoSansBold15);
  player.init();
  player.begin();
}

void gui_update()
{
    // ovr_label.foreground_color = right_pb.value > 1000 ? TFT_RED : TFT_DARK_DARK_GRAY;
    player.update();

#ifdef RM67162_DRIVER
    lcd_PushColors(0, 0, TFT_WIDTH, TFT_HEIGHT, (uint16_t*)lcd_buffer.getPointer());
#endif
}

void gui_update_task(void *arg)  
{
    while (1) 
    {
        gui_update();
        vTaskDelay(100 / portTICK_RATE_MS);
    }
}

void gui_run(int core) 
{
    xTaskCreate(gui_update_task, "gui_run", 2048, NULL, core, NULL);
}