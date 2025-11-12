#pragma once
// #include "Adafruit_TinyUSB.h"
#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"
#include "AudioTools/AudioLibs/AudioRealFFT.h"
#include "RadioStation.h"
#include "bands.h"

static constexpr RadioStation RadioStations[] { 
  {"SWISS Jazz",            "http://stream.srg-ssr.ch/m/rsj/mp3_128"},

  {"Radio Roks UA Ballads", "http://radio.d4ua.com:8800/roks_ballads"},
  {"Radio Roks UA",         "https://online.radioroks.ua/bak_RadioROKS"},
  {"Radio Roks UA HD",      "https://online.radioroks.ua/bak_RadioROKSTe_HD"},

 // {"Local",                 "http://192.168.1.85:49868/stream/swyh.mp3"},
  {"Asia Dream",            "https://igor.torontocast.com:1025/;.mp3"},
  {"KPop Radio",            "http://streamer.radio.co/s06b196587/listen"},
  {"Classic FM",            "http://media-ice.musicradio.com:80/ClassicFMMP3"},
  {"Lite Favorites",        "http://naxos.cdnstream.com:80/1255_128"},
  {"MAXXED Out",            "http://149.56.195.94:8015/steam"},
  {"SomaFM Xmas",           "http://ice2.somafm.com/christmas-128-mp3"},
  {"Veronica ",             "https://www.mp3streams.nl/zender/veronica/stream/11-mp3-128"}
};
static constexpr size_t RadioStationsCount = (sizeof(RadioStations) / sizeof(RadioStations[0]));

#ifdef ARDUINO
  #include "RadioStream.h"
  #define INIT_VOLUME 0.6

  I2SStream speakers_out;
  RadioStream in(RadioStations, RadioStationsCount, WIFI_SSID, WIFI_PASSWORD);
  // I2SStream in_i2s;  // Audio source

  // AudioStream* inputs[] = { &in, &in_i2s}; 
  // constexpr int inputs_count = sizeof(inputs) / sizeof(inputs[0]);
#else
  #include <iostream>

  #include "AudioTools/AudioLibs/PortAudioStream.h"
  #include "AudioTools/AudioLibs/Desktop/File.h"

  #define INIT_VOLUME 1.0

  PortAudioStream speakers_out;
  File in("./sound/file_example_MP3_700KB.mp3");
#endif

// NumberFormatConverterStream nfc(decoded_out);
//                                                           |-> AudioRealFFT
//                    |-> EncodedAudioStream -> MultiOutput -|-> VolumeStream -> PortAudioStream [I2SStream]
// In -> MultiOutput -|                                      |-> VolumeMeter
//                    |-> MetaDataOutput

AudioInfo info(44100, 2, 16);
LogarithmicVolumeControl lvc(0.1);

VolumeMeter meter_out;
AudioRealFFT fft_out; // or AudioKissFFT or others
MetaDataOutput metadata_out;

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

void fftResult(AudioFFTBase &fft) {
    //fft.frequencyToBin()
    for (auto i = 0; i < FTT_BANDS_COUNT; i++) {
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
  in.begin();
#else
  in.begin();
#endif

  // Decoder
  decoder.begin();

  // Out - VU
 // vu_out.begin();
  meter_out.begin(info);

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

  // Out - Mixer
  raw_out.add(decoder);
  raw_out.add(metadata_out);
  raw_out.begin();

  //all_out.add(vu_out);
  //all_out.add(speakers_out);
  all_out.add(volume_out);
  all_out.add(meter_out);
  all_out.add(fft_out);

  all_out.begin(info);
}

void loopAudio()
{
  copier.copy();

  // form.levelLeft.setValueOf(vu_out.value_left());
  // form.levelRight.setValueOf(vu_out.value_right());
  form.levelLeft.setValueOf(meter_out.volume(0));
  form.levelRight.setValueOf(meter_out.volume(1));
}