#pragma once
#include <SDL.h>
#include <SDL_ttf.h>

#define BUCKETS 30

int Freq_Bin[BUCKETS * 2]
{
  1, 5,
  6, 10,
  11, 15,
  16, 20,
  21, 25,
  26, 30,
  31, 35,
  36, 40,
  41, 45,
  46, 50,

  100, 110,
  111, 120,
  121, 125,
  126, 130,
  131, 135,
  136, 140,
  141, 145,
  146, 150,
  151, 155,
  156, 160,

  200, 210,
  211, 220,
  221, 225,
  226, 230,
  231, 235,
  236, 240,
  241, 245,
  246, 250,
  251, 255,
  256, 260
};

#define SAMPLES 1024
#define SAMPLES_DATA 1024 * 4 * 2

complex audio_signal[SAMPLES];
float spectrum[BUCKETS];

SDL_AudioDeviceID dev;

void run_fft(Uint8* stream, int start, int len)
{
    for (int i = 0; i < SAMPLES; i+=2)
    {
        auto sampleLeft = *(float*)(stream + start + i * 4);
        auto sampleRight = *(float*)(stream + start + i * 4 + 4);

        //sampleLeft *= 32767;

        //SDL_Log("%i L: %f R: %f", i, sampleLeft, sampleRight);

        audio_signal[i] = sampleLeft;
    }

    CFFT::Forward(audio_signal, SAMPLES);
    //auto SampleRate = 44100;

    //for (int j = 0; j < (SAMPLES / 2); j++)
    //{
    //    auto r = signal[j].re();
    //    auto i = signal[j].im();

    //    auto magnitude = sqrt(r * r + i * i);

    //    auto freq = j * SampleRate / SAMPLES;

    //    for (auto i = 0; i < BUCKETS; i++)
    //    {
    //        if (freq >= Freq_Bin[i] && freq <= Freq_Bin[i + 1])
    //        {
    //            if (magnitude > Spectrum[i])
    //            {
    //                Spectrum[i] = magnitude;
    //            }
    //        }
    //    }
    //}

    for (int i = 0, bin = 1; i < 30; i++, bin += 4)
    {
        analyzer.setBand(i, abs(audio_signal[bin].re()));
    }
}

void audioStreamCallback(void* userdata, Uint8* stream,  int len)
{
    //assert(len % 4 == 0);
    run_fft(stream, 0, len);
}



SDL_AudioSpec want, have;
SDL_AudioSpec wavSpec;
Uint32 wavLength;
Uint8* wavBuffer;

SDL_AudioDeviceID setupAudio(const char* file)
{
    //auto res = SDL_LoadWAV(file, &wavSpec, &wavBuffer, &wavLength);

    //if (res == 0) {
    //    SDL_Log("Failed to open audio: %s", SDL_GetError());
    //}

    SDL_memset(&want, 0, sizeof(want)); /* or SDL_zero(want) */
    want.freq = 44100;
    want.format = AUDIO_F32;
    want.channels = 2;
    want.samples = SAMPLES;
    want.callback = audioStreamCallback; /* you wrote this function elsewhere -- see SDL_AudioSpec for details */

    dev = SDL_OpenAudioDevice(NULL, 1, &want, &have, SDL_AUDIO_ALLOW_FORMAT_CHANGE);

    //auto success = SDL_QueueAudio(dev, wavBuffer, wavLength);

    //if (success!=0) {
    //    SDL_Log("Failed to queue audio: %s", SDL_GetError());
    //}

    if (dev == 0) {
        SDL_Log("Failed to open audio: %s", SDL_GetError());
    }
    else 
    {
        //if (have.format != want.format) { /* we let this one thing change. */
        //    SDL_Log("We didn't get Float32 audio format.");
        //}
    }

    SDL_Log("Default audio playback device: %s", SDL_GetAudioDeviceName(0, 1));

    return dev;
}

void listDevices(bool is_capture) {
    for (auto i = 0; i < SDL_GetNumAudioDevices(is_capture); i++) {
        SDL_Log("Audio device: %s", SDL_GetAudioDeviceName(i, is_capture));
    }
}