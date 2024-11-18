#pragma once

#include <TFT_eGUI/TFT_eGUI.h>

#include "NotoSansBold15.h"

TFT_eCassettePlayer player(&tft, TFT_HEIGHT, TFT_WIDTH, 0, 0);

void gui_init()
{
  tft.init();
  tft.setRotation(TFT_ROTATE);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);

  // tft.setFreeFont(&Orbitron_Medium_20);
  player.load_font(NotoSansBold15);
  player.init();
  player.begin();
}

void gui_update()
{
    // ovr_label.foreground_color = right_pb.value > 1000 ? TFT_RED : TFT_DARK_DARK_GRAY;

    // gui_pb_update(left_pb);
    // gui_pb_update(right_pb);

    // gui_label_update(ovr_label);
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