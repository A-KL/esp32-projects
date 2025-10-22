#pragma once

#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"
#include "AudioTools/AudioLibs/PortAudioStream.h"
#include "AudioTools/AudioLibs/AudioRealFFT.h"

#include <VuOutput.h>

#ifdef ARDUINO
  #include "AudioTools/Communication/AudioHttp.h"
  URLStream in(WIFI_SSID, WIFI_PASSWORD);
#else
  #include "AudioTools/AudioLibs/Desktop/File.h"
  File in("./sound/file_example_MP3_700KB.mp3");
#endif

AudioInfo info(44100, 1, 16);

// SineWaveGenerator<int16_t> sineWave(32000); // subclass of SoundGenerator with max amplitude of 32000
// GeneratedSoundStream<int16_t> in(sineWave);               // Stream generated from sine wave

VuMeter<int16_t> vu(AUDIO_VU_RATIO);
PortAudioStream out_speakers;
AudioRealFFT fft; // or AudioKissFFT or others

//MetaDataOutput metadata; // final output of metadata

MultiOutput out_mix;

MP3DecoderHelix helix;
EncodedAudioStream decoder(&out_mix, &helix); // output to decoder

//StreamCopy copier(out, in); // copy in to out
StreamCopy copier(decoder, in); // copy in to out

void startUI(void* args)
{
}

void setupWiFi() {
  delay(1000);
}

void setupControls() {
  delay(1000);
}

// display fft result
void fftResult(AudioFFTBase &fft)
{
    float diff;
    auto result = fft.result();

    if (result.magnitude>100)
    {
      //form.equalizer.bands.setBand(0, map(result.magnitude, 0, 255, 0, 3200));

      // printf(">> %f %f => %s diff: %f\r\n", 
      //   result.frequency, 
      //   result.magnitude, 
      //   result.frequencyAsNote(diff), 
      //   diff);

        // Serial.print(result.frequency);
        // Serial.print(" ");
        // Serial.print(result.magnitude);  
        // Serial.print(" => ");
        // Serial.print(result.frequencyAsNote(diff));
        // Serial.print( " diff: ");
        // Serial.println(diff);
    }
}

void setupAudio()
{
  // Setup FFT
  auto tcfg = fft.defaultConfig();
  tcfg.length = 4096;
  tcfg.channels = info.channels;
  tcfg.sample_rate = info.sample_rate;
  tcfg.bits_per_sample = info.bits_per_sample;
  tcfg.callback = &fftResult;
  fft.begin(tcfg);

  // open output
  auto config = out_speakers.defaultConfig();
  config.copyFrom(info);
  out_speakers.begin(config);

  out_mix.add(vu);
  out_mix.add(out_speakers);
  out_mix.add(fft);

  decoder.begin();

  out_mix.begin(info);

  in.begin();
  // Setup sine wave
  //sineWave.begin(info, N_B4);
}

void selectAudio(int dest, int src) {
}

void loopControls() {
}

void loopAudio()
{
  copier.copy();

  form.levelLeft.setValueOf(vu.value_left());
  form.levelRight.setValueOf(vu.value_right());
}