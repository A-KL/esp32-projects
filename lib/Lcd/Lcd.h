#pragma once

#ifndef TFT_SDL_SCALE
#define TFT_SDL_SCALE 3
#endif

/************************************************************************/
// Lovyan GFX - Graphics library for embedded devices
/************************************************************************/
#ifdef LGFX_BACKEND
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
    static TFT_eSPI tft (TFT_WIDTH, TFT_HEIGHT, TFT_SDL_SCALE);
  #else
    static TFT_eSPI tft;
  #endif
#endif

/************************************************************************/
// Adafruit's ILI9341 driver
/************************************************************************/
#ifdef AGFX_BACKEND
  #if defined(ILI9341_DRIVER) or (ILI9341_IPS_DRIVER)
    #include "Adafruit_ILI9341.h"
    using TFT_eSPI = Adafruit_ILI9341;
  #else
    #error Select on of the suported Adafruit_GFX display drivers
  #endif

  static TFT_eSPI tft(TFT_CS, TFT_DC, TFT_RES);
#endif

/************************************************************************/
// Bodmer/TFT_eSPI
/************************************************************************/
#ifdef TFT_eSPI_BACKEND
  #include <SPI.h>
  #include <TFT_eSPI.h>
  static TFT_eSPI tft;
#endif

