#pragma once

#if __has_include(<codec.h>) //&& not defined(USE_CODECS) // && defined(ARDUINO_ARCH_ESP32)
  #include "codec.h"
#endif

struct codec_info_t {
  bool muted;
  int volume;
  int address;
};

static codec_info_t codec_info { false, 50, 0x00 };

void setVolume(int16_t value);

void setupCodec()
{
# if defined(ARDUINO_ARCH_ESP32) and __has_include(<codec.h>)
  volume_out.setVolume(1.0);

  codec_init(codec_info.volume);

# endif
  setVolume(codec_info.volume);
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
  codec_mute(codec_info.muted);
#else 
  if (muted)
    volume_out.setVolume(0);
  else
    volume_out.setVolume(codec_info.volume/100.0);
#endif
}

void setMuted(bool muted)
{
  codec_info.muted = muted;

  form.volume.SetForeColor(codec_info.muted ? Color::Gray : Color::White);
  form.setIcon(5, codec_info.muted);
  
  setCodecMute(codec_info.muted);
}

void switchMuted()
{
  setMuted(!codec_info.muted);
}

// 0-100
void setVolume(int16_t value) 
{
  // Update UI
  auto dbs = value - 100;// (int)(value * 100 - 127.5);
  form.volume.SetTextF("%ddb", dbs);

  codec_info.volume = value;
  if (!codec_info.muted) {
    setCodecVolume(codec_info.volume);
  }
}