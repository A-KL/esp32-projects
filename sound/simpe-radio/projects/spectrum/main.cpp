#include "gui.h"

void setup(){
  Serial.begin(115200);

  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);

  //tft.setFreeFont(&Orbitron_Medium_20);
  tft.loadFont(NotoSansBold15);

  tft.fillScreen(TFT_BLACK);

  gui_init();

  gui_run(0);
}

void loop(){
}