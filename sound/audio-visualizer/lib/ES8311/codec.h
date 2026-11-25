#pragma once

#if not defined(USE_CODEC_ES8311)

#include "codec_es8311.h"

inline esp_err_t codec_init(int16_t volume = PLATFORM_INIT_VOLUME, int32_t sample_rate = 44100)
{
  return codec_es8311_init(volume, sample_rate);
}

inline esp_err_t codec_mute(bool mute)
{
  return codec_es8311_mute(mute);
}

inline esp_err_t codec_volume(float volume)
{
  return codec_es8311_volume(volume);
}

inline esp_err_t codec_volume_percentage(int16_t volume)
{
  return codec_es8311_volume_percentage(volume);
}

inline esp_err_t codec_deinit()
{
  return codec_es8311_deinit();
}

#endif