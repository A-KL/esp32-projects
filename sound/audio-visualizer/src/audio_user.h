#pragma once

static bool is_muted = false;
static int16_t volume = 50;

void setCodecVolume(float value)
{
#if defined(USE_CODEC_VOLUME) && defined(ARDUINO)
  #include "codec.h"
  codec_volume_percentage(value);
#else 
  volume_out.setVolume(value/100.0);
#endif
}

void setCodecMute(bool muted)
{
#if defined(USE_CODEC_VOLUME) && defined(ARDUINO)
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

  form.volume.setForecolor( is_muted ? Color::Gray : Color::White);
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
  log_e("Volume: %i", value);

  // Update UI
  auto dbs = value - 100;// (int)(value * 100 - 127.5);
  form.volume.setTextF("%ddb", dbs);

  volume = value;
  if (!is_muted) {
    setCodecVolume(volume);
  }
}



void changeAudioInput()
{
  // inputs[selected_input]->end();
  // selected_input++;

  // if (inputs_count <= selected_input) {
  //   selected_input = 0;
  // }
  // inputs[selected_input]->begin();
}

void selectAudio(int dest, int src) 
{
  // static auto _selectedAudioSource = 0;
  // static auto _selectedAudioTarget = 1;
  // selectAudio(_selectedAudioTarget, _selectedAudioSource);
  // form.setIcon(_selectedAudioTarget, 1);
  // form.setIcon(_selectedAudioSource + 2, 1);
}