#pragma once

#if defined(LGFX_BACKEND) or (TFT_eSPI_BACKEND) or __has_include(<LGFX_TFT_eSPI.h>) or __has_include(<TFT_eSPI.h>)
  #include "LovyanGFXCanvas.h"
  #define LcdCanvas LovyanGFXCanvas

#elif defined(Arduino_GFX_BACKEND) or __has_include(<Arduino_GFX_Library.h>)
  #include "ArduinoGFXCanvas.h"
  using LcdCanvas = ArduinoGFXCanvas<TFT_eSPI>;

#elif defined(AGFX_BACKEND)
  #include "AGFXCanvas.h"
  using LcdCanvas = AGFXCanvas<TFT_eSPI>;

#else
  #error Include on of the suported display libraries: LovyanGFX, TFT_eSPI, ArduinoGFX or AdafruitGFX
#endif