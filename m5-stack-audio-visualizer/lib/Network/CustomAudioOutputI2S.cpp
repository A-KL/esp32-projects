#include "CustomAudioOutputI2S.h"

bool CustomAudioOutputI2S::ConsumeSample(int16_t sample[2])
{
    _sampleCallback(sample[0], sample[1]);
    
    return AudioOutputI2S::ConsumeSample(sample);
}