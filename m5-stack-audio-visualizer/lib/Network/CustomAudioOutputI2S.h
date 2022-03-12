#pragma once

#include "AudioOutputI2S.h"

class CustomAudioOutputI2S : public AudioOutputI2S
{
  public:
    CustomAudioOutputI2S(int port=0, int output_mode=EXTERNAL_I2S, int dma_buf_count = 8, int use_apll=APLL_DISABLE)
        :   AudioOutputI2S(port, output_mode, dma_buf_count, use_apll), _sampleCallback(DummyCallback)
    {}

    virtual bool ConsumeSample(int16_t sample[2]) override;
    
    typedef void (*sampleCBFn)(int16_t left, int16_t right);
    void OnSampleCallback(sampleCBFn f) { _sampleCallback = f; }

    private:
        sampleCBFn _sampleCallback;

        static void DummyCallback(int16_t left, int16_t right)
        {}
};