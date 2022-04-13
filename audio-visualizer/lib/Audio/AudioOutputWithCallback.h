#pragma once

#include "AudioOutputI2S.h"
#include "AudioOutputSPDIF.h"
#include "AudioFrame.h"

typedef void (*SampleDelegate)(const AudioFrame& frame);

class AudioOutputWithCallback : public AudioOutput
{
public:
    AudioOutputWithCallback(AudioOutput* innerAudioOutput)
        : _audioOutput(innerAudioOutput)
    {}

    virtual bool ConsumeSample(int16_t sample[2]) override;

    virtual bool SetRate(int hz) override;
    virtual bool SetBitsPerSample(int bits) override;
    virtual bool SetChannels(int chan) override;
    virtual bool SetGain(float f) override;
    virtual bool begin() override;

    virtual bool stop() override;
    virtual void flush() override;
    virtual bool loop() override;

    void SampleCallback(SampleDelegate delegate);

    virtual ~AudioOutputWithCallback() 
    {
        delete _audioOutput;
    };

private:
    AudioOutput* _audioOutput;
    SampleDelegate _onSampleDelegate;
};