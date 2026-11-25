#pragma once

#if defined(ARDUINO_ARCH_ESP32) and defined(USE_CODEC_ES8311)
# include <codec_es8311.h>
#else 
  inline void codec_es8311_init(int16_t volume, int32_t sample_rate){ }
#endif

#if defined(ARDUINO_ARCH_ESP32) and defined(USE_CODEC_ES9018K2M)
# include <codec_es9018k2m.h>
#else 
  inline void codec_es9018k2m_init(int16_t volume, int32_t sample_rate){ }
#endif

void codec_init_all()
{
  codec_es8311_init(70, 44100);
  codec_es9018k2m_init(70, 44100);
}