#pragma once

/************************************************************************/
// Lovyan GFX - Graphics library for embedded devices
/************************************************************************/
#ifdef LGFX_BACKEND
  #ifdef LGFX_AUTODETECT
    #include <LGFX_AUTODETECT.hpp>
  #endif

  #ifdef ILI9341_DRIVER
    #include "LGFX_ESP32_ILI9341.hpp"
  #endif

    #include <LGFX_TFT_eSPI.h>
#endif

/************************************************************************/
// Adafruit's ILI9341 driver
/************************************************************************/
#ifdef AGFX_BACKEND
  #ifdef ILI9341_DRIVER //__has_include ( header-name )
    #include "Adafruit_ILI9341.h"
    using TFT_eSPI = Adafruit_ILI9341;
  #endif
#endif

/************************************************************************/
// Bodmer/TFT_eSPI
/************************************************************************/
#ifdef TFT_eSPI_BACKEND
    #include <TFT_eSPI.h>
#endif