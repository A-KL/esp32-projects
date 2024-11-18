#pragma once

#include <TFT_eSPI.h>
#include <TFT_eGUI/TFT_eGUI.h>

#include "NotoSansBold15.h"

TFT_eSprite canvas = TFT_eSprite(&tft);
TFT_eCassettePlayer player(&canvas, TFT_HEIGHT, TFT_WIDTH, 0, 0);

void gui_init()
{
  tft.init();
  tft.setRotation(TFT_ROTATE);
  tft.setSwapBytes(true);

  // //tft.setFreeFont(&Orbitron_Medium_20);
  tft.loadFont(NotoSansBold15);
  tft.fillScreen(TFT_BLACK);

  player.init();
  player.begin();
}