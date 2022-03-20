#include "CustomAudioOutputI2S.h"

void CustomAudioOutputI2S::SampleCallback(SampleDelegate delegate) 
{ 
    _sampleDelegate = delegate; 
}

bool CustomAudioOutputI2S::ConsumeSample(int16_t sample[2])
{
    if (_sampleDelegate) {
        AudioFrame frame {sample[0], sample[1]};
        _sampleDelegate(frame);
    }
    return AudioOutputI2S::ConsumeSample(sample);
}