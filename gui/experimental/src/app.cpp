#ifdef LGFX_AUTODETECT
	#include <LovyanGFX.h>
	#include <LGFX_AUTODETECT.hpp>
#else
#include <TFT_eSPI.h>
#include <TFT_eSPI_GFX.h>
using LGFX = TFT_eSPI_GFX;
using LGFX_Sprite = TFT_eSprite;
#endif

//#include <TFTShape.h>

#include "experimental.h"

#if defined ( SDL_h_ )
static LGFX lcd ( 320, 240, 2 );
#else
static LGFX lcd;
#endif


void setup(void)
{
  lcd.init();
  lcd.fillScreen(TFT_BLUE);

	test_chevron(lcd, 10, 10, 300, 60);

	auto w = 200;
	auto h = w * 0.7;

	test_card_v2(lcd, "20%", "progress", 10, 80, w, h);
}

void loop(void)
{
	//delay(10);
}