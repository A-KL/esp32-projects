//#include <Arduino.h>
//#include <map>

#ifdef LGFX_AUTODETECT
  #include <LGFX_AUTODETECT.hpp>
#endif

#ifdef ARDUINO
  #include "LGFX_ESP32_ILI9341.hpp"
#endif

#include <LGFX_TFT_eSPI.h>

#if defined ( SDL_h_ )
  static TFT_eSPI lcd (TFT_WIDTH, TFT_HEIGHT, 3);
#else
  static TFT_eSPI lcd;
#endif

#include "NotoSansBold15.h"

void setup() 
{
#ifdef ARDUINO
  Serial.begin(115200);
#endif

  lcd.init();
  // lcd.setColorDepth(16);
  // lcd.startWrite();
  lcd.fillScreen(TFT_BLACK);

  lcd.loadFont(NotoSansBold15); // If I try to load the font

  lcd.setTextColor(TFT_CYAN);
  lcd.drawCenterString("TEST string!", 50, 50); // this line doesn't draw anything

  lcd.fillRect(120, 20, 100, 100, TFT_BLUE); // nor this one
}

void loop() 
{
  lgfx::delay(100);
}