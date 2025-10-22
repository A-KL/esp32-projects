#pragma once

//using namespace lgfx;

#include "AudioTools.h"
//#include "AudioTools/Communication/AudioHttp.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"


#include <AudioTools/AudioLibs/PortAudioStream.h>
#include <AudioTools/AudioLibs/AudioRealFFT.h> // using RealFFT

#include <VuOutput.h>

AudioRealFFT fft; // or AudioKissFFT or others

//URLStream url(WIFI_SSID, WIFI_PASSWORD);

MultiOutput out_mix;
VuMeter<int16_t> vu(AUDIO_VU_RATIO);

AudioInfo info(44100, 1, 16);
SineWaveGenerator<int16_t> sineWave(32000);               // subclass of SoundGenerator with max amplitude of 32000
GeneratedSoundStream<int16_t> in(sineWave);               // Stream generated from sine wave
PortAudioStream out;                                        // On desktop we use 
//StreamCopy copier(out, in); // copy in to out

StreamCopy copier(out_mix, in); // copy in to out

void startUI(void* args)
{
}

void setupWiFi()
{
  delay(1000);
}

void setupControls()
{
  delay(1000);
}

// display fft result
void fftResult(AudioFFTBase &fft)
{
    float diff;
    auto result = fft.result();

    if (result.magnitude>100)
    {
      form.equalizer.bands.setBand(0, map(result.magnitude, 0, 255, 0, 3200));

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
  auto config = out.defaultConfig();
  config.copyFrom(info);
  out.begin(config);

  out_mix.add(vu);
  out_mix.add(out);
  out_mix.add(fft);

  out_mix.begin(info);
  // Setup sine wave
  sineWave.begin(info, N_B4);
}

void selectAudio(int dest, int src)
{

}

void loopControls()
{
  
}

void loopAudio()
{
  if (out) {
    copier.copy();

    form.levelLeft.setValueOf(vu.value_left());
    form.levelRight.setValueOf(vu.value_right());
  }
}