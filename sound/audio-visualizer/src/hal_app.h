#pragma once

#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"
#include "AudioTools/AudioLibs/AudioRealFFT.h"

#include <VuOutput.h>

#ifdef ARDUINO
  #include "AudioTools/Communication/AudioHttp.h"

  I2SStream speakers_out;
  URLStream in(WIFI_SSID, WIFI_PASSWORD);
#else
  #include "AudioTools/AudioLibs/PortAudioStream.h"
  #include "AudioTools/AudioLibs/Desktop/File.h"

  PortAudioStream speakers_out;
  File in("./sound/file_example_MP3_700KB.mp3");
#endif

AudioInfo info(44100, 1, 16);

// SineWaveGenerator<int16_t> sineWave(32000); // subclass of SoundGenerator with max amplitude of 32000
// GeneratedSoundStream<int16_t> in(sineWave);               // Stream generated from sine wave
// NumberFormatConverterStream nfc(decoded_out);

//                                                           |-> AudioRealFFT
//                    |-> EncodedAudioStream -> MultiOutput -|-> PortAudioStream
// In -> MultiOutput -|                                      |-> VuMeter
//                    |-> MetaDataOutput


VuMeter<int16_t> vu_out(AUDIO_VU_RATIO);
AudioRealFFT fft_out; // or AudioKissFFT or others
MetaDataOutput metadata_out; // final output of metadata

MultiOutput all_out;
MultiOutput raw_out;

MP3DecoderHelix helix;
EncodedAudioStream decoder(&all_out, &helix);

StreamCopy copier(raw_out, in);

void log_init()
{
#ifdef ARDUINO
  Serial.begin(115200);
#endif
  AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Warning);
}

void startUI(void* args)
{
}

void setupWiFi() {
  delay(1000);
}

void setupControls() {
  delay(1000);
}

void fftResult(AudioFFTBase &fft)
{
    //float diff;
    //auto result = fft.result();
    auto d = fft.size();

    for (auto i=1; i<31; i++)
    {
        auto d = fft.magnitude(i);
        form.equalizer.bands.setBand(i-1, d); //map(result.magnitude, 0, 255, 0, 3200)
    }
}

void printMetaData(MetaDataType type, const char* str, int len)
{
  printf("==> %s: %s\r\n", toStr(type), str);
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

  // Out - Mixer
  raw_out.add(decoder);
  raw_out.add(metadata_out);
  raw_out.begin();

  all_out.add(vu_out);
  all_out.add(speakers_out);
  all_out.add(fft_out);

  all_out.begin(info);

  //sineWave.begin(info, N_B4);
}

void selectAudio(int dest, int src) {
}

void loopControls() {
}

void loopAudio()
{
  copier.copy();

  form.levelLeft.setValueOf(vu_out.value_left());
  form.levelRight.setValueOf(vu_out.value_right());
}