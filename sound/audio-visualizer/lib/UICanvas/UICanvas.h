#pragma once

#if defined(TFT_eSPI_BACKEND) or (TFT_eSPI_BACKEND)
  #include "LovyanGFXCanvas.h"
  #define TFT_Canvas LovyanGFXCanvas

#elif defined(Arduino_GFX_BACKEND)
  #include "ArduinoGFXCanvas.h"
  using TFT_Canvas = ArduinoGFXCanvas<TFT_eSPI>;

#elif defined(AGFX_BACKEND)
  #include "AGFXCanvas.h"
  #define TFT_Canvas AGFXCanvas<TFT_eSPI>

#else
  #error Select on of the suported backend display drivers
#endif