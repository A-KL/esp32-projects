#pragma once

namespace ui
{
  inline unsigned long millis()
  {
  #ifdef ARDUINO_ARCH_ESP32
      return ::millis();
  #elif __has_include(<LGFX_TFT_eSPI.h>)
      return lgfx::millis();
  #else
      return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  #endif
  }
}