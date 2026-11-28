#pragma once

#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"
#include "AudioTools/AudioCodecs/CodecAACHelix.h"
#include "AudioTools/FFT/AudioRealFFT.h"

# include "RadioStation.h"
# include "RadioStream.h"
# include "bands.h"
# include "VuOutput.h"
# include "outputs.h"

//                                            I2SStream-In -|                |-> AudioRealFFT
//                                    |-> EncodedAudioStream -> MultiOutput -|-> VolumeStream -> NumberFormatConverterStreamV2 -> PortAudioStream [I2SStream]
// RadioStream [File] -> MultiOutput -|                                      |-> VolumeMeter
//                                    |-> MetaDataOutput
//

const std::vector<RadioStation> Playlist = {
  {"SWISS Jazz",           "http://stream.srg-ssr.ch/m/rsj/mp3_128"},
  {"SS",                   "http://livestreaming-node-1.srg-ssr.ch/srgssr/rsj/mp3/128"},
  {"Radio Roks UA",        "http://online.radioroks.ua/bak_RadioROKS"},
  {"KissFM",               "http://tavr.tvstitch.com/KissFM"},
  {"Classic FM",           "http://media-ice.musicradio.com:80/ClassicFMMP3"},
  {"Lite Favorites",       "http://naxos.cdnstream.com:80/1255_128"},
  {"TRANCE 21",            "https://mscp3.live-streams.nl:8022/lowstream"},
  {"TRANCE IS STAR RADIO", "http://listen2.myradio24.com/tisradio"},
  {"Veronica",             "https://www.mp3streams.nl/zender/veronica/stream/11-mp3-128",},
  {"Smooth Jazz4",         "http://jazz.streeemer.com/listen/jazz/radio.aac"}
};

#ifdef ARDUINO_ARCH_ESP32
  #include "AudioTools/AudioLibs/I2SCodecStream.h"
  #include "StaticDriverPins.h"
  #include "AudioDriver.h"

  #define Q(x) #x
  #define QUOTE(x) Q(x)
  #define MAKE_INCLUDE_NAME(x) QUOTE(x.h)

  #ifdef USE_PORT0_CODEC
  # if __has_include(MAKE_INCLUDE_NAME(USE_PORT0_CODEC))
  #  include MAKE_INCLUDE_NAME(USE_PORT0_CODEC)
  # endif
  #else
  # define USE_PORT0_CODEC NoDriver
  #endif

  #ifdef USE_PORT1_CODEC
  # if __has_include(MAKE_INCLUDE_NAME(USE_PORT1_CODEC))
  #  include MAKE_INCLUDE_NAME(USE_PORT1_CODEC)
  # endif
  #else
  # define USE_PORT1_CODEC NoDriver
  #endif

  #ifdef USE_PORT2_CODEC
  # if __has_include(MAKE_INCLUDE_NAME(USE_PORT2_CODEC))
  #  include MAKE_INCLUDE_NAME(USE_PORT2_CODEC)
  # endif
  #else
  # define USE_PORT2_CODEC NoDriver
  #endif

  StaticDriverPins pins[I2S_COUNT];
  I2SCodecStream outs[I2S_COUNT];
  AudioBoard* audio_boards[I2S_COUNT];

  void setupCodecs() 
  {
  # if defined(ARDUINO_ARCH_ESP32)
    AudioDriver* driver = &USE_PORT0_CODEC;

    for (auto i=0; i<I2S_COUNT; i++) {
        pins[i].configure(i);
        if (i == 1) driver = &USE_PORT1_CODEC;
        if (i == 2) driver = &USE_PORT2_CODEC; 
        if (i > 2) driver = &NoDriver;
        audio_boards[i] = new AudioBoard(driver, &pins[i]);
        outs[i].setBoard(audio_boards[i]);
    }
  # endif
  }

  I2SStream speakers_out[I2S_COUNT];
  // I2SStream stream_in;
#else
  #include <iostream>
  #include "AudioTools/AudioLibs/PortAudioStream.h"
  PortAudioStream speakers_out[I2S_COUNT];
  // File radio_in("./sound/file_example_MP3_700KB.mp3");
  // File stream_in("./sound/M1F1-int16WE-AFsp.wav");
#endif

AudioInfo info_in(48000, 2, 16);
AudioInfo info_out(48000, 2, 32);

RadioStream radio_in(Playlist, WIFI_SSID, WIFI_PASSWORD);

VuMeter vu_out;
AudioRealFFT fft_out;
MetaDataOutput metadata_out;

MultiOutput all_out;
MultiOutput raw_out;

// Equalizer3Bands eq(convert);
NumberFormatConverterStream convert(speakers_out[I2S_DEFAULT]);

LogarithmicVolumeControl lvc(0.1);
VolumeStream volume_out(convert);

MP3DecoderHelix mp3;
AACDecoderHelix aac;
WAVDecoder wav;
MultiDecoder multi;

EncodedAudioStream decoder(&all_out, &multi);

StreamCopy copier(raw_out, radio_in); // Radio
// StreamCopy copier; //(all_out, stream_in); // USB

void onFFTResult(AudioFFTBase &fft) 
{
    //fft.frequencyToBin()
    for (auto i = 0; i < FTT_BANDS_COUNT; i++) {
        auto bin_index = ftt_bin_map[i]; 
        auto bin_value = fft.magnitude(bin_index);
       form.equalizer.bands.setBand(i, sqrt(bin_value) * 8.0f);
      // form.equalizer.bands.setBand(i, bin_value);
    }
}

void printMetadata(MetaDataType type, const char* str, int len)
{
  LOGW("%s: %s\n", toStr(type), str);
  if (type == Title) {
    form.track.SetText(str);
  }
}

void selectAudioOutput(const uint8_t index)
{
#if defined(I2S_COUNT) and (I2S_COUNT>1)
  convert.setStream(speakers_out[index]);
  form.setIcon(0, index == 0);
  form.setIcon(1, index == 1);
#endif
}

void showRadioInfo(URLStream& stream)
{
  // ice-audio-info
  // form.track.SetText(radio_in.getTitle());
  form.track.SetText(stream.getReplyHeader("icy-name"));

  auto content_type = stream.getReplyHeader("content-type");
  form.encoder.SetText(media_types[std::string(content_type)].c_str());

  auto bitrate1 = stream.getReplyHeader("icy-br");
  auto bitrate2 = stream.getReplyHeader("icy-bitrate");
  form.quality.SetText(bitrate1 == nullptr ? bitrate2 : bitrate1);
}

// 0 = radio
// 1 = i2s
void setupAudio(const int mode = 0)
{
  // Input: File or stream
  radio_in.begin(1);
  showRadioInfo(radio_in);

  // Decoder
  multi.addDecoder(mp3, "audio/mpeg");
  multi.addDecoder(aac, "audio/aac");
  multi.addDecoder(wav, "audio/vnd.wave");
  decoder.begin();

  // Out - VU
  vu_out.begin(info_in);

  // Out - Speakers
  // for (auto& out : outs) {
  //   out.setAudioInfo(info_out);
  //   out.begin();
  // }
  output_streams_config(speakers_out, info_out, I2S_COUNT);
  selectAudioOutput(I2S_DEFAULT);

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
  cfg_fft.callback = &onFFTResult;
  fft_out.begin(cfg_fft);

  // Out - metadata 
  // metadata_out.setCallback(printMetadata);
  // metadata_out.begin(radio_in.httpRequest());

  // Out - Volume
  volume_out.setVolumeControl(lvc);

  // Out - Mixer
  raw_out.add(decoder);
  raw_out.add(metadata_out);
  raw_out.begin();

  // Out - Convert
  convert.begin(info_in, info_out);

  // Out - Mixer
  all_out.add(volume_out);
  all_out.add(vu_out);
  all_out.add(fft_out);
  all_out.begin();

  // Copier - Start
  // copier.setActive(false);
  // if (mode == 0) { 
  //   copier.begin(raw_out, radio_in); // Radio
  // } else {
  //   // copier.begin(all_out, speakers_out); // I2SStream Full Duplex
  // }
  // copier.setActive(true);
}

void loopAudio()
{
  copier.copy();

  auto left = vu_out.value_left() * 5;
  auto right = vu_out.value_right() * 5;
  
  form.levelLeft.setValueOf(left);
  form.levelRight.setValueOf(right);
}
