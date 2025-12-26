#pragma once

#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"
#include "AudioTools/AudioLibs/AudioRealFFT.h"
#include "RadioStation.h"
#include "bands.h"

#include "VuOutput.h"

//                                        I2SStream-In                       |-> AudioRealFFT
//                                    |-> EncodedAudioStream -> MultiOutput -|-> VolumeStream -> PortAudioStream [I2SStream]
// RadioStream [File] -> MultiOutput -|                                      |-> VolumeMeter
//                                    |-> MetaDataOutput
//

static constexpr RadioStation RadioStations[] { 
  {"SWISS Jazz",            "http://stream.srg-ssr.ch/m/rsj/mp3_128"},
  {"Radio Roks UA Ballads", "http://radio.d4ua.com:8800/roks_ballads"},
  {"Radio Roks UA",         "http://online.radioroks.ua/bak_RadioROKS"},
  {"Radio Roks UA HD",      "http://online.radioroks.ua/bak_RadioROKSTe_HD"},
  {"Classic FM",            "http://media-ice.musicradio.com:80/ClassicFMMP3"},
  {"Lite Favorites",        "http://naxos.cdnstream.com:80/1255_128"},
  {"MAXXED Out",            "http://149.56.195.94:8015/steam"},
 // {"SomaFM Xmas",           "http://ice2.somafm.com/christmas-128-mp3"},
  {"Veronica ",             "http://www.mp3streams.nl/zender/veronica/stream/11-mp3-128"}
};
static constexpr size_t RadioStationsCount = (sizeof(RadioStations) / sizeof(RadioStations[0]));

#ifdef ARDUINO
  #include "RadioStream.h"
  #define INIT_VOLUME 0.8

  I2SStream speakers_out;
  RadioStream radio_in(RadioStations, RadioStationsCount, WIFI_SSID, WIFI_PASSWORD);
  // I2SStream stream_in;
  // NumberFormatConverterStream nfc(decoded_out);
#else
  #include <iostream>

  #include "AudioTools/AudioLibs/PortAudioStream.h"
  #include "AudioTools/AudioLibs/Desktop/File.h"

  #define INIT_VOLUME 1.0

  PortAudioStream speakers_out;
  File radio_in("./sound/file_example_MP3_700KB.mp3");
  File stream_in("./sound/M1F1-int16WE-AFsp.wav");
#endif

// AudioInfo info(48000, 2, 32);
AudioInfo info_in(44100, 2, 16);
AudioInfo info_out(44100, 2, 32);

LogarithmicVolumeControl lvc(0.1);

VolumeMeter meter_out;
AudioRealFFT fft_out;
MetaDataOutput metadata_out;

MultiOutput all_out;
MultiOutput raw_out;

NumberFormatConverterStream convert(speakers_out); // write the 16bit data to fc

MP3DecoderHelix helix;
VolumeStream volume_out(convert);
EncodedAudioStream decoder(&all_out, &helix);

// StreamCopy copier(raw_out, radio_in); // Radio
StreamCopy copier; //(all_out, stream_in); // USB

void log_init()
{
#ifdef ARDUINO
  Serial.begin(115200);
#endif
  AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Warning);
}

void fftResult(AudioFFTBase &fft) {
    //fft.frequencyToBin()
    for (auto i = 0; i < FTT_BANDS_COUNT; i++) {
        auto bin_index = ftt_bin_map[i]; 
        auto bin_value = fft.magnitude(bin_index);
       // form.equalizer.bands.setBand(i, sqrt(bin_value)*15);
      //form.equalizer.bands.setBand(i, bin_value * 3);
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

// 0 = radio
// 1 = i2s
void setupAudio(const int mode = 0)
{
  // Input: File or stream
  radio_in.begin();

#ifdef ARDUINO
  form.track.setText(radio_in.getTitle());
#endif

  // Decoder
  decoder.begin();

  // Out - VU
  meter_out.begin(info_in);

  // Out - Speakers
#ifdef ARDUINO
  auto config = speakers_out.defaultConfig(mode == 0 ? TX_MODE : RXTX_MODE);
  config.copyFrom(info_out);
  config.pin_ws      = I2S_WS;
  config.pin_bck     = I2S_BCK;
  config.pin_data    = I2S_SD;
  config.is_master   = I2S_MASTER;
  config.pin_data_rx = I2S_IN_SD;
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
  //tcfg.window_function = new BufferedWindow(new Hamming());
  //tcfg.window_function = new Hamming();
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
  convert.begin(16, 32);

  //all_out.add(speakers_out);
  all_out.add(volume_out);
  //all_out.add(vu_out);
  all_out.add(meter_out);
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
  
  form.levelLeft.setValueOf(meter_out.volume(0) * 2);
  form.levelRight.setValueOf(meter_out.volume(1) * 2);
}