#ifdef LGFX_AUTODETECT
  #include <LGFX_AUTODETECT.hpp>
#endif

#ifdef ARDUINO
  #include "LGFX_ESP32_IPS_ILI9341.hpp"
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
  //lcd.setRotation(1);
  lcd.setColorDepth(16);
  lcd.startWrite();
  lcd.fillScreen(TFT_BLACK);

  auto left = 10;
  auto top = 10;
  auto w = 200;
  auto h = 100;

  TFT_eSprite sprite(&lcd);
  sprite.createSprite(w, 1);
  // sprite.setColorDepth(16);
	// sprite.setSwapBytes(true);
	// sprite.fillSprite(TFT_BLACK);

  sprite.drawLine(0, 0, 100, 0, TFT_YELLOW);
  sprite.pushSprite(left, top);

  sprite.deleteSprite();
  sprite.createSprite(w, 2);
  // sprite.setColorDepth(16);
	// sprite.setSwapBytes(true);
	// sprite.fillSprite(TFT_BLACK);

  //sprite.fillSprite(TFT_BLACK);

  sprite.drawLine(0, 0, 100, 0, TFT_GREEN);
  sprite.pushSprite(left, top + 5);

  sprite.deleteSprite();
}

void loop() 
{
  lgfx::delay(100);
}