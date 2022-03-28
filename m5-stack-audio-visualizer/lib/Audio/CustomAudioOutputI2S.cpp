#include "CustomAudioOutputI2S.h"

void AudioOutputWithCallback::SampleCallback(SampleDelegate delegate) 
{ 
    _onSampleDelegate = delegate; 
}

bool AudioOutputWithCallback::ConsumeSample(int16_t sample[2])
{
    if (_onSampleDelegate) {
        AudioFrame frame {sample[0], sample[1]};
        _onSampleDelegate(frame);
    }

    if (_audioOutput != NULL) {
       return _audioOutput->ConsumeSample(sample);
    }

    return false;
}

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

void AudioOutputSPDIFWithCallback::SampleCallback(SampleDelegate delegate) 
{ 
    _sampleDelegate = delegate; 
}

bool AudioOutputSPDIFWithCallback::ConsumeSample(int16_t sample[2])
{
    if (_sampleDelegate) {
        AudioFrame frame {sample[0], sample[1]};
        _sampleDelegate(frame);
    }
    return AudioOutputSPDIF::ConsumeSample(sample);
}