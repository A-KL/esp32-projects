#pragma once

// #include "Adafruit_TinyUSB.h"

#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"
#include "AudioTools/AudioLibs/AudioRealFFT.h"
#include <VuOutput.h>
#include "bands.h"
#include "RadioStation.h"

#ifdef ARDUINO
  #include "AudioTools/Communication/AudioHttp.h"
  #include "radio.h"
  #define INIT_VOLUME 0.6

  I2SStream speakers_out;
  RadioStream in(WIFI_SSID, WIFI_PASSWORD);
  I2SStream in_i2s;  // Audio source

  AudioStream* inputs[] = { &in, &in_i2s}; 
  constexpr int inputs_count = sizeof(inputs) / sizeof(inputs[0]);
#else
  #include <iostream>

  #include "AudioTools/AudioLibs/PortAudioStream.h"
  #include "AudioTools/AudioLibs/Desktop/File.h"

  #define INIT_VOLUME 1.0
  PortAudioStream speakers_out;
  File in("./sound/file_example_MP3_700KB.mp3");

 // AudioStream* inputs[] = { &in }; 
#endif

// SineWaveGenerator<int16_t> sineWave(32000);    // subclass of SoundGenerator with max amplitude of 32000
// GeneratedSoundStream<int16_t> in(sineWave);    // Stream generated from sine wave
// NumberFormatConverterStream nfc(decoded_out);

//                                                           |-> AudioRealFFT
//                    |-> EncodedAudioStream -> MultiOutput -|-> VolumeStream -> PortAudioStream [I2SStream]
// In -> MultiOutput -|                                      |-> VuMeter
//                    |-> MetaDataOutput

AudioInfo info(44100, 2, 16);
LogarithmicVolumeControl lvc(0.1);

VuMeter<int16_t> vu_out(AUDIO_VU_RATIO);
VolumeMeter meter;

AudioRealFFT fft_out; // or AudioKissFFT or others
MetaDataOutput metadata_out;

MultiOutput all_out;
MultiOutput raw_out;

MP3DecoderHelix helix;
VolumeStream volume_out(speakers_out);
EncodedAudioStream decoder(&all_out, &helix);

StreamCopy copier(raw_out, in);

// auto cfg_meter = meter.defaultConfig();
// cfg_meter.copyFrom(info);
// out.begin(cfg_meter);

void log_init()
{
#ifdef ARDUINO
  Serial.begin(115200);
#endif
  AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Warning);
}

void fftResult(AudioFFTBase &fft) {
    //fft.frequencyToBin()
    for (auto i=0; i < FTT_BANDS_COUNT; i++)
    {
        auto bin_index = ftt_bin_map[i]; 
        auto bin_value = fft.magnitude(bin_index);
        form.equalizer.bands.setBand(i, sqrt(bin_value)*15);
    }
}

void printMetaData(MetaDataType type, const char* str, int len)
{
#ifdef ARDUINO
  log_w("%s: %s", toStr(type), str);
#else
  printf("==> %s: %s\r\n", toStr(type), str);
#endif
  if (type == Title) {
    form.track.setText(str);
  }
}

void setupAudio()
{
  // Input: File or stream
#ifdef ARDUINO
  in.setPlaylist(RadioStations, RadioStationsCount);
  in.begin();
#else
  in.begin();
#endif

  // Decoder
  decoder.begin();

  // Out - VU
 // vu_out.begin();

  // Out - Speakers
#ifdef ARDUINO
  auto config = speakers_out.defaultConfig(TX_MODE);
  config.copyFrom(info);
  config.pin_ws = I2S_WS;
  config.pin_bck = I2S_BCK;
  config.pin_data = I2S_SD;
  config.is_master = I2S_MASTER;
#else
  auto config = speakers_out.defaultConfig();
  config.copyFrom(info);
#endif
  speakers_out.begin(config);

  // Out - FFT
  auto tcfg = fft_out.defaultConfig();
  tcfg.length = 4096;
  tcfg.channels = info.channels;
  tcfg.sample_rate = info.sample_rate;
  tcfg.bits_per_sample = info.bits_per_sample;
  //tcfg.window_function = new BufferedWindow(new Hamming());
  tcfg.window_function = new Hamming();
  tcfg.callback = &fftResult;
  fft_out.begin(tcfg);

  // Out - metadata 
  metadata_out.setCallback(printMetaData);
  metadata_out.begin();

  // Out - Volume

  volume_out.setVolumeControl(lvc);
  volume_out.setVolume(INIT_VOLUME);

  // auto cfg_meter = meter.defaultConfig();
  // cfg_meter.copyFrom(info);
  meter.begin(info);

  // Out - Mixer
  raw_out.add(decoder);
  raw_out.add(metadata_out);
  raw_out.begin();

  //all_out.add(vu_out);
  //all_out.add(speakers_out);
  all_out.add(volume_out);
  all_out.add(meter);
  all_out.add(fft_out);

  all_out.begin(info);
}

inline void loopAudio()
{
  copier.copy();

  // form.levelLeft.setValueOf(vu_out.value_left());
  // form.levelRight.setValueOf(vu_out.value_right());

  form.levelLeft.setValueOf(meter.volume(0));
  form.levelRight.setValueOf(meter.volume(1));
}

// ----------------------------------------------------//
static bool is_muted;
constexpr const char* input_labels[] = { "rds", "USB" };
static int selected_input = 0;

void setVolume(long long value) 
{
    if (!is_muted && value == 0) {
      form.volume.setForecolor(Color::Gray);
      form.setIcon(5, true);
      volume_out.setVolume(0);
      is_muted = true;
      return;
    }

    if (is_muted && value == 0) {
      form.volume.setForecolor(Color::White);
      auto dbs = (int)(value* 0.5 - 127.5);       
      form.volume.setTextF("%ddb", dbs);
      form.setIcon(5, false);
      volume_out.setVolume(0.5);
      is_muted = false;
      return;
    }


    if (volume_out.setVolume(value / 255.0)){
      auto dbs = (int)(value* 0.5 - 127.5);
      form.volume.setTextF("%ddb", dbs);
    }

    // volumeDac(value);
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
