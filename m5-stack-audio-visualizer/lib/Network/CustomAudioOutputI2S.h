#pragma once

#include "AudioOutputI2S.h"

typedef struct __attribute__((packed)) {
    int16_t left;
    int16_t right;
} AudioFrame;

typedef void (*SampleDelegate)(const AudioFrame& frame);

class CustomAudioOutputI2S : public AudioOutputI2S
{
public:
    CustomAudioOutputI2S(int port=0, int output_mode=EXTERNAL_I2S, int dma_buf_count = 8, int use_apll=APLL_DISABLE)
        :   AudioOutputI2S(port, output_mode, dma_buf_count, use_apll)
    {}

    virtual bool ConsumeSample(int16_t sample[2]) override;

    void SampleCallback(SampleDelegate delegate);

private:
    SampleDelegate _sampleDelegate;
};