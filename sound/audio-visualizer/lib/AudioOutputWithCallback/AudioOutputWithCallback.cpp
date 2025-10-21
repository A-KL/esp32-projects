
#include "AudioOutputI2S.h"
#include "AudioOutputSPDIF.h"

#include "AudioOutputWithCallback.h"

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

    return AudioOutput::ConsumeSample(sample);
}

bool AudioOutputWithCallback::SetRate(int hz){
    if (_audioOutput != NULL) {
       return _audioOutput->SetRate(hz);
    }
    return AudioOutput::SetRate(hz);
}

bool AudioOutputWithCallback::SetBitsPerSample(int bits){
    if (_audioOutput != NULL) {
       return _audioOutput->SetBitsPerSample(bits);
    }
    return AudioOutput::SetBitsPerSample(bits);
}

bool AudioOutputWithCallback::SetChannels(int chan){
    if (_audioOutput != NULL) {
       return _audioOutput->SetChannels(chan);
    }
    return AudioOutput::SetChannels(chan);
}

bool AudioOutputWithCallback::SetGain(float f){
    if (_audioOutput != NULL) {
       return _audioOutput->SetGain(f);
    }
    return AudioOutput::SetGain(f);
}

bool AudioOutputWithCallback::begin(){
    if (_audioOutput != NULL) {
       return _audioOutput->begin();
    }
    return AudioOutput::begin();
}

bool AudioOutputWithCallback::stop(){
    if (_audioOutput != NULL) {
       return _audioOutput->stop();
    }
    return AudioOutput::stop();
}

void AudioOutputWithCallback::flush(){
    if (_audioOutput != NULL) {
       return _audioOutput->flush();
    }
    return AudioOutput::flush();
}

bool AudioOutputWithCallback::loop(){
    if (_audioOutput != NULL) {
       return _audioOutput->loop();
    }
    return AudioOutput::loop();
}