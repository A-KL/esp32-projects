#pragma once

#include <AudioFileSource.h>
#include <AudioFileSourceBuffer.h>
#include <AudioFileSourceICYStream.h>
#include <AudioGeneratorTalkie.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>
#include <AudioOutputI2SNoDAC.h>
#include "CustomAudioOutputI2S.h"

class InternetRadio
{
public:
    InternetRadio();

    inline void SetUrl(const char* url)
    {
        _url = url;
    }

    void Loop();

    void StartPlaying(const char* url);

    void StopPlaying();

    typedef void (*sampleCBFn)(int16_t left, int16_t right);
    void OnSampleCallback(sampleCBFn f);

private:
    const char *_url;
    float _gain = 4.0;
    const int _bufferSize = 16 * 1024; // buffer in byte, default 16 * 1024 = 16kb

    AudioGeneratorTalkie* _talkie;          
    AudioGeneratorMP3* _mp3;
    AudioFileSourceICYStream* _stream;
    AudioFileSourceBuffer* _buffer;
    CustomAudioOutputI2S* _output;

    static void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string);

    static void StatusCallback(void *cbData, int code, const char *string);
};