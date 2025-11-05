#pragma once

#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"
#include "AudioTools/AudioLibs/AudioRealFFT.h"

#include <VuOutput.h>

#ifdef ARDUINO
  #include "AudioTools/Communication/AudioHttp.h"
  #define INIT_VOLUME 0.6

  I2SStream speakers_out;
  URLStream in(WIFI_SSID, WIFI_PASSWORD);
#else
  #include <iostream>

  #include "AudioTools/AudioLibs/PortAudioStream.h"
  #include "AudioTools/AudioLibs/Desktop/File.h"

  #define INIT_VOLUME 1.0
  PortAudioStream speakers_out;
  File in("./sound/file_example_MP3_700KB.mp3");
#endif

// SineWaveGenerator<int16_t> sineWave(32000);    // subclass of SoundGenerator with max amplitude of 32000
// GeneratedSoundStream<int16_t> in(sineWave);    // Stream generated from sine wave
// NumberFormatConverterStream nfc(decoded_out);

//                                                           |-> AudioRealFFT
//                    |-> EncodedAudioStream -> MultiOutput -|-> VolumeStream -> PortAudioStream
// In -> MultiOutput -|                                      |-> VuMeter
//                    |-> MetaDataOutput

AudioInfo info(44100, 2, 16);
LogarithmicVolumeControl lvc(0.1);

VuMeter<int16_t> vu_out(AUDIO_VU_RATIO);
AudioRealFFT fft_out; // or AudioKissFFT or others
MetaDataOutput metadata_out; // final output of metadata

MultiOutput all_out;
MultiOutput raw_out;

MP3DecoderHelix helix;
VolumeStream volume_out(speakers_out);
EncodedAudioStream decoder(&all_out, &helix);

StreamCopy copier(raw_out, in);

void log_init()
{
#ifdef ARDUINO
  Serial.begin(115200);
#endif
  AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Warning);
}

void setupWiFi() {
  delay(1000);
}

// 4096 / 2 = 2048 bins
// 44100 / 2048 = 21.5 Hz per bin
int ftt_bin_map[FTT_BANDS_COUNT] = {
  1, // 21.5 Hz
  2, // 43 Hz
  3, // 64 Hz
  5, // 105 Hz
  6,
  7, // 157 Hz
  10,
  12, // 250 Hz
  15,
  18, // 400 Hz
  23,
  29, // 630 Hz
  35,
  46, // 1kHz
  55,
  74, // 1k6
  100,
  116, // 2k5
  150,
  186, // 4k
  200,
  250,
  293, // 6k3
  350,
  400,
  465, // 10k
  500,
  550,
  600,
  744, // 16k
};   

void fftResult(AudioFFTBase &fft)
{
    //fft.magnitudesFast()
    //fft.frequencyToBin()
    //auto d = fft.result();
    
    for (auto i=0; i<FTT_BANDS_COUNT; i++)
    {
        auto bin_index = ftt_bin_map[i]; 
        auto bin_value = fft.magnitude(bin_index);
       //form.equalizer.bands.setBand(i, bin_value); //map(result.magnitude, 0, 255, 0, 4700)
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
  in.begin("http://stream.srg-ssr.ch/m/rsj/mp3_128","audio/mp3");
#else
  in.begin();
#endif

  // Decoder
  decoder.begin();

  // Out - VU
  vu_out.begin();

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
  tcfg.callback = &fftResult;
  fft_out.begin(tcfg);

  // Out - metadata 
  metadata_out.setCallback(printMetaData);
  metadata_out.begin();

  // Out - Volume

  volume_out.setVolumeControl(lvc);
  volume_out.setVolume(INIT_VOLUME);

  // Out - Mixer
  raw_out.add(decoder);
  raw_out.add(metadata_out);
  raw_out.begin();

  all_out.add(vu_out);
  //all_out.add(speakers_out);
  all_out.add(volume_out);
  all_out.add(fft_out);

  all_out.begin(info);

  //sineWave.begin(info, N_B4);
}

void setVolume(long long value) 
{
    if (volume_out.setVolume(value / 255.0)){
      auto dbs = (int)(value* 0.5 - 127.5);
      form.volume.setTextF("%ddb", dbs);
    }

    // volumeDac(value);
}

void selectAudio(int dest, int src) {
}

inline void loopAudio()
{
  copier.copy();

  form.levelLeft.setValueOf(vu_out.value_left());
  form.levelRight.setValueOf(vu_out.value_right());
}