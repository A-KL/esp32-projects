#pragma once

#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"
#include "AudioTools/FFT/AudioRealFFT.h"
#include "RadioStation.h"
#include "bands.h"
#include "VuOutput.h"

#if __has_include(<codec.h>)
  #include "codec.h"
#endif

//                                        I2SStream-In                       |-> AudioRealFFT
//                                    |-> EncodedAudioStream -> MultiOutput -|-> VolumeStream -> PortAudioStream [I2SStream]
// RadioStream [File] -> MultiOutput -|                                      |-> VolumeMeter
//                                    |-> MetaDataOutput
//

const std::vector<RadioStation> Playlist = {
  {"SWISS Jazz",           "http://stream.srg-ssr.ch/m/rsj/mp3_128"},
  {"Radio Roks UA",        "http://online.radioroks.ua/bak_RadioROKS"},
  {"KissFM",               "https://tavr.tvstitch.com/KissFM"},
  {"Classic FM",           "http://media-ice.musicradio.com:80/ClassicFMMP3"},
  {"Lite Favorites",       "http://naxos.cdnstream.com:80/1255_128"},
  {"TRANCE 21",            "https://mscp3.live-streams.nl:8022/lowstream"},
  {"TRANCE IS STAR RADIO", "https://listen2.myradio24.com/tisradio"},
  {"Veronica ",            "http://www.mp3streams.nl/zender/veronica/stream/11-mp3-128"}
};

#ifndef INIT_VOLUME
  #define INIT_VOLUME 1.0
#endif

#ifdef ARDUINO_ARCH_ESP32
  #include "RadioStream.h"

  I2SStream speakers_out;
  RadioStream radio_in(Playlist, WIFI_SSID, WIFI_PASSWORD);
  // I2SStream stream_in;
#else
  #include <iostream>

  #include "AudioTools/AudioLibs/PortAudioStream.h"
  #include "RadioStream.h"
  // #include "AudioTools/Disk/FileSystem.h"

  PortAudioStream speakers_out;
  // File radio_in("./sound/file_example_MP3_700KB.mp3");
  RadioStream radio_in(Playlist, WIFI_SSID, WIFI_PASSWORD);
  // File stream_in("./sound/M1F1-int16WE-AFsp.wav");
#endif

// AudioInfo info(48000, 2, 32);
AudioInfo info_in(44100, 2, 16);
AudioInfo info_out(44100, 2, 32);

LogarithmicVolumeControl lvc(0.1);

VuMeter vu_out;
VolumeMeter meter_out;
AudioRealFFT fft_out;
MetaDataOutput metadata_out;

MultiOutput all_out;
MultiOutput raw_out;

NumberFormatConverterStream convert(speakers_out);

MP3DecoderHelix helix;
VolumeStream volume_out(convert);
EncodedAudioStream decoder(&all_out, &helix);

// StreamCopy copier(raw_out, radio_in); // Radio
StreamCopy copier; //(all_out, stream_in); // USB

void log_init()
{
#ifdef ARDUINO
  Serial.begin(115200);
  #if ARDUINO_USB_CDC_ON_BOOT
  // A zero timeout keeps optional logging from delaying application startup
  // when USB CDC is not connected or the host is not draining output.
  Serial.setTxTimeoutMs(0);
  #endif
#endif
  AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Warning);
}

void fftResult(AudioFFTBase &fft) 
{
    //fft.frequencyToBin()
    for (auto i = 0; i < FTT_BANDS_COUNT; i++) {
        auto bin_index = ftt_bin_map[i]; 
        auto bin_value = fft.magnitude(bin_index);
       form.equalizer.bands.setBand(i, sqrt(bin_value) * 8.0f);
      // form.equalizer.bands.setBand(i, bin_value);
    }
}

void printMetaData(MetaDataType type, const char* str, int len)
{
  log_w("%s: %s", toStr(type), str);

  if (type == Title) {
    form.track.SetText(str);
  }
}

// 0 = radio
// 1 = i2s
void setupAudio(const int mode = 0)
{
# if defined(ARDUINO_ARCH_ESP32) and __has_include(<codec.h>)
  // Hardware init
  codec_init();
# endif

  // Input: File or stream
  radio_in.select(1);
  form.track.SetText(radio_in.getTitle());

  // Decoder
  decoder.begin();

  // Out - VU
  meter_out.begin(info_in);

  // Out - Speakers
#ifdef ARDUINO_ARCH_ESP32
  auto config = speakers_out.defaultConfig(mode == 0 ? TX_MODE : RXTX_MODE);
  config.copyFrom(info_out);
  config.pin_ws      = I2S_WS;
  config.pin_bck     = I2S_BCK;
  config.pin_data    = I2S_SD;
#if defined(I2S_MASTER)
  config.is_master   = I2S_MASTER;
#else
  config.is_master   = true;
#endif
#if defined(I2S_MCLK_MULTIPLE)
  config.mclk_multiple = I2S_MCLK_MULTIPLE;
#endif
#if defined(I2S_MCK)
  config.pin_mck     = I2S_MCK;
#endif
#if defined(I2S_IN_SD)
  config.pin_data_rx = I2S_IN_SD;
#endif
#else
  auto config = speakers_out.defaultConfig();
  config.copyFrom(info_out);
#endif
  speakers_out.begin(config);

  // Out - FFT
  auto tcfg = fft_out.defaultConfig();
  tcfg.length = 4096;
  tcfg.channels = info_in.channels;
  tcfg.sample_rate = info_in.sample_rate;
  tcfg.bits_per_sample = info_in.bits_per_sample;
  // tcfg.window_function = new BufferedWindow(new Hamming());
  tcfg.window_function = new FlatTop();
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

  // Out - Convert
  convert.begin(16, 16);

  vu_out.begin(info_in); //??

  //all_out.add(speakers_out);
  all_out.add(volume_out);
  all_out.add(vu_out); //??
  // all_out.add(meter_out);
  all_out.add(fft_out);

  all_out.begin(info_in);

  copier.setActive(false);
  if (mode == 0) { 
    copier.begin(raw_out, radio_in); // Radio
  } else {
    copier.begin(all_out, speakers_out); // I2SStream Full Duplex
  }
  copier.setActive(true);
}

void loopAudio()
{
  copier.copy();

  auto left = vu_out.value_left() * 5;
  auto right = vu_out.value_right() * 5;
  
  form.levelLeft.setValueOf(left);
  form.levelRight.setValueOf(right);
}