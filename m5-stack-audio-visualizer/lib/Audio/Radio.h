#pragma once

#include <iostream>

#include <AudioFileSource.h>
#include <AudioFileSourceBuffer.h>
#include <AudioFileSourceICYStream.h>
#include <AudioGeneratorTalkie.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>
#include <AudioOutputI2SNoDAC.h>
#include "CustomAudioOutputI2S.h"

struct RadioStation 
{
    const char* Name;
    const char* Url;

    friend std::ostream& operator<<(std::ostream& os, const RadioStation& station);
};

typedef void (*StreamDelegate)(const char *type, const char *value);

class InternetRadio
{
public:
    InternetRadio();

    inline float Gain() const
    {
        return _gain;
    }

    inline void SetGain(float gain)
    {
        _gain = gain;

        if (_output != NULL) {
            //_output->SetGain(_gain*0.05);
        }
    }    

    inline void SetUrl(const char* url)
    {
        _url = url;
    }

    void Loop();

    void Play(const char* url);

    void Stop();


    void SampleCallback(SampleDelegate delegate);
   
    static StreamDelegate StreamChanged;

private:
    const char *_url;
    float _gain = 4.0;
    const int _bufferSize = 16 * 1024; // buffer in byte, default 16 * 1024 = 16kb

    AudioGeneratorTalkie* _talkie;          
    AudioGeneratorMP3* _mp3;
    AudioFileSourceICYStream* _stream;
    AudioFileSourceBuffer* _buffer;
    AudioOutputSPDIFWithCallback* _output;

    static void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string);

    static void StatusCallback(void *cbData, int code, const char *string);
};