#pragma once
// #include "Adafruit_TinyUSB.h"
#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"
#include "AudioTools/AudioLibs/AudioRealFFT.h"
#include "RadioStation.h"
#include "bands.h"
#include "StreamRouter.h"
#include "VuOutput.h"

static constexpr RadioStation RadioStations[] { 
  {"SWISS Jazz",            "http://stream.srg-ssr.ch/m/rsj/mp3_128"},
  {"Radio Roks UA Ballads", "http://radio.d4ua.com:8800/roks_ballads"},
  {"Radio Roks UA",         "http://online.radioroks.ua/bak_RadioROKS"},
  {"Radio Roks UA HD",      "http://online.radioroks.ua/bak_RadioROKSTe_HD"},
  {"Classic FM",            "http://media-ice.musicradio.com:80/ClassicFMMP3"},
  {"Lite Favorites",        "http://naxos.cdnstream.com:80/1255_128"},
  {"MAXXED Out",            "http://149.56.195.94:8015/steam"},
  {"SomaFM Xmas",           "http://ice2.somafm.com/christmas-128-mp3"},
  {"Veronica ",             "http://www.mp3streams.nl/zender/veronica/stream/11-mp3-128"}
};
static constexpr size_t RadioStationsCount = (sizeof(RadioStations) / sizeof(RadioStations[0]));

//                                        I2SStream-In                       |-> AudioRealFFT
//                                    |-> EncodedAudioStream -> MultiOutput -|-> VolumeStream -> PortAudioStream [I2SStream]
// RadioStream [File] -> MultiOutput -|                                      |-> VolumeMeter
//                                    |-> MetaDataOutput
//

#ifdef ARDUINO
  #include "RadioStream.h"
  
  #define INIT_VOLUME 0.8

  I2SStream audio_out;
  RadioStream radio_in(RadioStations, RadioStationsCount, WIFI_SSID, WIFI_PASSWORD);
  I2SStream stream_in;
#else
  #include <iostream>

  #include "AudioTools/AudioLibs/PortAudioStream.h"
  #include "AudioTools/AudioLibs/Desktop/File.h"

  #define INIT_VOLUME 1.0

  PortAudioStream audio_out;
  File radio_in("./sound/file_example_MP3_700KB.mp3");
  File stream_in("./sound/file_example_MP3_700KB.mp3");
#endif

// AudioInfo info(48000, 2, 32);
AudioInfo info(44100, 2, 16);

LogarithmicVolumeControl lvc(0.1);
VuMeter<int32_t> meter_old;
VolumeMeter meter_out;

AudioRealFFT fft_out;
MetaDataOutput metadata_out;

MultiOutput all_out;
MultiOutput raw_out;

MP3DecoderHelix helix;
//VolumeStream volume_out(speakers_out);
EncodedAudioStream decoder(&all_out, &helix);

// StreamCopy copier(raw_out, in1); // Radio
StreamCopy copier; //(all_out, in2); // USB

void log_init()
{
#ifdef ARDUINO
  Serial.begin(115200);
#endif
  AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Info);
}

void on_fft_results(AudioFFTBase &fft) {
    //fft.frequencyToBin()
    for (auto i = 0; i < FTT_BANDS_COUNT; i++) {
        auto bin_index = ftt_bin_map[i]; 
        auto bin_value = fft.magnitude(bin_index);
       // form.equalizer.bands.setBand(i, sqrt(bin_value)*15);
        form.equalizer.bands.setBand(i, bin_value);
    }
}

void on_print_metadata(MetaDataType type, const char* str, int len)
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

void setupOutputs() {

}

void endAudio() {
  // Copy
  copier.setActive(false);

  // INs
  radio_in.end();
  stream_in.end();

  // OUTs
  audio_out.end();
}

AudioOutput* beginAudioOut() 
{
  // Out - Speakers
#ifdef ARDUINO
  auto config_out = audio_out.defaultConfig(TX_MODE);
  config_out.copyFrom(info);
  config_out.pin_ws = I2S_WS;
  config_out.pin_bck = I2S_BCK;
  config_out.pin_data = I2S_SD;
  config_out.is_master = I2S_MASTER;
#else
  // Out - I2SStream
  auto config_out = audio_out.defaultConfig();
  config_out.copyFrom(info);
#endif
  audio_out.begin(config_out);

  // Out - VU
  meter_out.begin(info);
  meter_old.begin(info);

  // Out - FFT
  auto tcfg = fft_out.defaultConfig();
  tcfg.copyFrom(info);
  tcfg.length = 4096;
  //tcfg.window_function = new BufferedWindow(new Hamming());
  tcfg.window_function = new Hamming();
  tcfg.callback = &on_fft_results;
  //fft_out.begin(tcfg);

  // Out - Volume
  // volume_out.setVolumeControl(lvc);
  // volume_out.setVolume(INIT_VOLUME);

  // Out - Mixer 
  all_out.begin();

  return &all_out;
}

#ifdef ARDUINO
AudioStream*
#else
Stream*
#endif
  beginAudioIn() {
  // Input: File or I2SStream
  #ifdef ARDUINO
    // I2SStream
    auto config = stream_in.defaultConfig(RX_MODE);
    config.copyFrom(info);
    config.is_master = false;
    config.pin_ws = I2S_IN_WS;
    config.pin_bck = I2S_IN_BCK;
    config.pin_data = I2S_IN_SD;
    stream_in.begin(config);
  #else
    // File
    stream_in.begin();
  #endif

  return &stream_in;
}

#ifdef ARDUINO
AudioStream*
#else
Stream*
#endif
 beginRadioIn() {
  // Input: File or stream
  radio_in.begin();

  // Metadata
  metadata_out.setCallback(on_print_metadata);
  metadata_out.begin();

  // Decoder
  decoder.begin();

  // Out
  raw_out.begin();

  return &radio_in;
}

void beginAudio(const int mode) {
  //endAudio();
  if (mode == 0) // Radio
  {
    form.setIcon(2, true);
  #ifdef ARDUINO
    form.track.setText(radio_in.getTitle());
  #endif
    auto from = beginRadioIn();
    auto to = beginAudioOut();
    copier.begin(raw_out, *from);
  } 
  else if ( mode == 1) // USB
  {
    form.setIcon(4, true);
    auto from1 = beginAudioIn();
    auto to1 = beginAudioOut();
    copier.begin(*to1, *from1);
  }
  else if (mode == 2)  // S/PDIF pass
  {
    form.setIcon(0, true);
  } 
  else 
  {
    return;
  }

  //copier.setActive(true);
}

void setupAudio()
{  
  // Mixers
  
  // MP3 output
  raw_out.add(decoder);
  raw_out.add(metadata_out);

  // WAV output
  all_out.add(audio_out); // all_out.add(volume_out);
  // all_out.add(meter_old);
  all_out.add(meter_out);
  all_out.add(fft_out);

  beginAudio(0);
}

void loopAudio()
{
  copier.copy();

  // if (last > meter_out.volumeRatio()) {
  //   last -= 32*meter_out.volumeRatio();
  // } else {
  //   last += 50;
  // }
  //last = USHRT_MAX * meter_out.volume(0) / NumberConverter::maxValue(24);;
  // log_w();
  
  // if (millis() - last_update > 10) {
  //   // form.levelLeft.setValueOf(meter_out.volume(0));
  //   // form.levelRight.setValueOf(meter_out.volume(1));
    form.levelLeft.setValueOf(USHRT_MAX * meter_out.volume(0) / NumberConverter::maxValue(32));
    form.levelRight.setValueOf(USHRT_MAX * meter_out.volume(1) / NumberConverter::maxValue(32));
  //   last_update = millis();
  // }
}