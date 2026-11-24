#pragma once

static bool is_muted = false;
static int16_t volume = 50;

void setVolume(int16_t value);

void setupCodec()
{
# if defined(ARDUINO_ARCH_ESP32) and __has_include(<codec.h>)
  volume_out.setVolume(1.0);
  codec_init(volume);
# endif
  setVolume(volume);
}

void setCodecVolume(int16_t value)
{
#if defined(ARDUINO) && __has_include(<codec.h>)
  codec_volume_percentage(value);
#else 
  volume_out.setVolume(value/100.0);
#endif
}

void setCodecMute(bool muted)
{
#if defined(ARDUINO) && defined(USE_CODEC_VOLUME)
  #include "codec.h"
  codec_mute(muted);
#else 
  if (muted)
    volume_out.setVolume(0);
  else
    volume_out.setVolume(volume/100.0);
#endif
}

void setMuted(bool muted)
{
  is_muted = muted;

  form.volume.SetForeColor( is_muted ? Color::Gray : Color::White);
  form.setIcon(5, is_muted);
  
  setCodecMute(is_muted);
}

void switchMuted()
{
  setMuted(!is_muted);
}

// 0-100
void setVolume(int16_t value) 
{
  // Update UI
  auto dbs = value - 100;// (int)(value * 100 - 127.5);
  form.volume.SetTextF("%ddb", dbs);

  volume = value;
  if (!is_muted) {
    setCodecVolume(volume);
  }
}