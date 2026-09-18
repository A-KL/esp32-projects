#pragma once

/************************************************************************/
// Lovyan GFX - Graphics library for embedded devices
/************************************************************************/
#ifdef LGFX_BACKEND
  #ifndef TFT_SDL_SCALE
    #define TFT_SDL_SCALE 3
  #endif

  #ifdef LGFX_AUTODETECT
    #include <LGFX_AUTODETECT.hpp>
  #endif
  
  #if not defined ( SDL_h_ )
    #include "LGFX_ESP32_LCD.hpp"
  #endif
  #include <LGFX_TFT_eSPI.h>

  #include "TFT_eSprite_GFX.h"
  #define TFT_eSprite TFT_eSprite_GFX

  #if defined ( SDL_h_ )
    #if TFT_ROTATE % 2 == 0
      #define SDL_HEIGHT TFT_WIDTH
      #define SDL_WIDTH  TFT_HEIGHT
    #else
      #define SDL_HEIGHT TFT_HEIGHT
      #define SDL_WIDTH  TFT_WIDTH
    #endif
    static TFT_eSPI tft (SDL_WIDTH, SDL_HEIGHT, TFT_SDL_SCALE);
  #else
    static TFT_eSPI tft;
  #endif
#endif

/************************************************************************/
// Adafruit's Graphics library driver
/************************************************************************/
#ifdef AGFX_BACKEND

  #if defined(ILI9341_DRIVER) or defined(ILI9341_IPS_DRIVER)
    #include "Adafruit_ILI9341.h"
    Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
    using TFT_eSPI = Adafruit_ILI9341;

  #elif defined(ST7789V_DRIVER) or defined(ST7789_DRIVER)
    #include <Adafruit_ST7789.h>
    Adafruit_ST7789 tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
    using TFT_eSPI = Adafruit_ST7789;

  #else
    #error Select on of the suported Arduino_GFX display drivers
  #endif

  namespace lgfx { void delayMicroseconds(uint32_t ms) { delay(ms); } }
#endif

/************************************************************************/
// Arduino Graphics library driver
/************************************************************************/
#ifdef Arduino_GFX_BACKEND   

  #if defined(ST7789_DRIVER) or defined(ST7789V_DRIVER)
    #include <Arduino_GFX_Library.h>
    Arduino_HWSPI bus(TFT_DC, TFT_CS, TFT_SCLK, TFT_MOSI, TFT_MISO, &SPI, false);
    Arduino_ST7789 tft(&bus, TFT_RST, 0 /* rotation */, false /* IPS */, TFT_WIDTH /* width */, TFT_HEIGHT /* height */, 0 /* col offset 1 */, 0 /* row offset 1 */, 0 /* col offset 2 */, 80 /* row offset 2 */);
    using TFT_eSPI = Arduino_ST7789;
  #else
    #error Select on of the suported Arduino_GFX display drivers
  #endif
  
  namespace lgfx { void delayMicroseconds(uint32_t ms) { delay(ms); } }
#endif

/************************************************************************/
// Bodmer/TFT_eSPI
/************************************************************************/
#ifdef TFT_eSPI_BACKEND
  #include <SPI.h>
  #include <TFT_eSPI.h>
  static TFT_eSPI tft;
  namespace lgfx { void delayMicroseconds(uint32_t ms) { delay(ms); } }
#endif

