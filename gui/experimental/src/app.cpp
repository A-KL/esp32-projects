#include <stdio.h>
#define LGFX_USE_V1

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
#include "abstract-topographic-contour-small.h"
#include "experimental.h"

#if defined ( SDL_h_ )

static LGFX lcd ( 320, 240, 2 );
#else
static LGFX lcd;
#endif

#include <iostream>

// struct img_t
// {
//   const unsigned char* data;
//   const unsigned int len;
//   const uint32_t w;
//   const uint32_t h;
// };

// const img_t test_img = img_t { 
// 	abstract_topographic_contour_small,       
// 	921738,       
// 	630, 
// 	340 
// };

lgfx::FileWrapper bitmap_file;

void setup(void)
{
  lcd.init();
  lcd.fillScreen(TFT_BLUE);

	// if(!bitmap_file.open("./image/abstract-topographic-contour-small.bmp"))
	// {
	// 	std::cout << "File Failed" << std::endl;
	// }
}

void loop(void)
{
	static int top = 0;
	static int d = 1;

	//auto res = 
	//lcd.drawBitmap(0, 0, (uint8_t*)abstract_topographic_contour_small, 320, 240, TFT_BLACK);
	lcd.pushImage(-72, -196, 360, 640, image_abstract_topographic_contour_small_pixels);
	 // ("/Users/anatolii.klots/Documents/Sources/esp32-projects/gui/experimental/image/abstract-topographic-contour-vector.bmp", 0, top);
	
	//lcd.pushImage(0, 0, 320, 240, abstract_topographic_contour_small, true);
	// if (!res)
	// {
	// 	std::cout << "Failed: " << res << std::endl;
	// }

	//bitmap_file.seek(0);
	top+=d;
	if (top>lcd.height()) {
		d *= -1 ;
	}

	test_chevron(lcd, 10, 10, 300, 60);

	auto w = 200;
	auto h = w * 0.7;

	test_card_v2(lcd, "20%", "progress", 10, 80, w, h);

	//lgfx::delay(100);
}