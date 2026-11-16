#pragma once

#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"
#include "AudioTools/FFT/AudioRealFFT.h"
#include "RadioStation.h"
#include "RadioStream.h"
#include "bands.h"
#include "VuOutput.h"
#include "outputs.h"

#if __has_include(<codec.h>)
  #include "codec.h"
#endif

//                                        I2SStream-In                       |-> AudioRealFFT
//                                    |-> EncodedAudioStream -> MultiOutput -|-> VolumeStream -> PortAudioStream [I2SStream]
// RadioStream [File] -> MultiOutput -|                                      |-> VolumeMeter
//                                    |-> MetaDataOutput
//

const std::vector<RadioStation> Playlist = {
  {"SWISS Jazz",           "https://stream.srg-ssr.ch/m/rsj/mp3_128"},
  {"Radio Roks UA",        "http://online.radioroks.ua/bak_RadioROKS"},
  {"KissFM",               "http://tavr.tvstitch.com/KissFM"},
  {"Classic FM",           "http://media-ice.musicradio.com:80/ClassicFMMP3"},
  {"Lite Favorites",       "http://naxos.cdnstream.com:80/1255_128"},
  {"TRANCE 21",            "https://mscp3.live-streams.nl:8022/lowstream"},
  {"TRANCE IS STAR RADIO", "http://listen2.myradio24.com/tisradio"},
  {"Veronica ",            "https://www.mp3streams.nl/zender/veronica/stream/11-mp3-128"}
};

#ifndef INIT_VOLUME
  #define INIT_VOLUME 1.0
#endif

#ifdef ARDUINO_ARCH_ESP32
  I2SStream speakers_out[I2S_COUNT];
  // I2SStream stream_in;
#else
  #include <iostream>
  #include "AudioTools/AudioLibs/PortAudioStream.h"
  // #include "AudioTools/Disk/FileSystem.h"
  PortAudioStream speakers_out[I2S_COUNT];
  // File radio_in("./sound/file_example_MP3_700KB.mp3");
  // File stream_in("./sound/M1F1-int16WE-AFsp.wav");
#endif

// AudioInfo info(48000, 2, 32);
AudioInfo info_in(44100, 2, 16);
AudioInfo info_out(44100, 2, 32);

RadioStream radio_in(Playlist, WIFI_SSID, WIFI_PASSWORD);

LogarithmicVolumeControl lvc(0.1);

VuMeter vu_out;
AudioRealFFT fft_out;
MetaDataOutput metadata_out;

MultiOutput all_out;
MultiOutput raw_out;

// Equalizer3Bands eq(convert);
NumberFormatConverterStream convert(speakers_out[0]);

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
  AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Info);
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
  LOGW("%s: %s", toStr(type), str);
  if (type == Title) {
    form.track.SetText(str);
  }
}

void selectAudioOutput(uint8_t index)
{
#if defined(I2S_COUNT) and (I2S_COUNT>1)
  convert.setOutput(speakers_out[index]);
  form.setIcon(0, index == 0);
  form.setIcon(1, index == 1);
#endif
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
  radio_in.select(4);
  form.track.SetText(radio_in.getTitle());

  // Decoder
  decoder.begin();

  // Out - VU
  vu_out.begin(info_in);

  // Out - Speakers
  output_streams_config(speakers_out, I2S_COUNT);
  selectAudioOutput(0);

  // Out - EQ
  // cfg_eq = eq.defaultConfig();
  // cfg_eq.setAudioInfo(cfg); // use channels, bits_per_sample and sample_rate from kit
  // cfg_eq.gain_low = 0.5; 
  // cfg_eq.gain_medium = 0.5;
  // cfg_eq.gain_high = 1.0;
  // eq.begin(cfg_eq);

  // Out - FFT
  auto cfg_fft = fft_out.defaultConfig();
  cfg_fft.length = 4096;
  cfg_fft.channels = info_in.channels;
  cfg_fft.sample_rate = info_in.sample_rate;
  cfg_fft.bits_per_sample = info_in.bits_per_sample;
  // cfg_fft.window_function = new BufferedWindow(new Hamming());
  cfg_fft.window_function = new FlatTop();
  cfg_fft.callback = &fftResult;
  fft_out.begin(cfg_fft);

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

  // Out - Mixer
  all_out.add(volume_out);
  all_out.add(vu_out);
  all_out.add(fft_out);

  all_out.begin(info_in);

  // Copier - Start
  copier.setActive(false);
  if (mode == 0) { 
    copier.begin(raw_out, radio_in); // Radio
  } else {
    // copier.begin(all_out, speakers_out); // I2SStream Full Duplex
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
