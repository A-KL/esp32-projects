#pragma once

#define USE_A2DP
#define USE_I2S

#include <AudioFileSource.h>
#include <AudioFileSourceBuffer.h>
#include <AudioFileSourceICYStream.h>
#include <AudioGeneratorTalkie.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>
#include <AudioOutputI2SNoDAC.h>
#include "BluetoothA2DPSource.h"
#include "AudioOutputWithCallback.h"

// #include "AudioConfig.h"
// #include "AudioTimer/AudioTimer.h"
// #include "AudioTools/AudioTypes.h"
// #include "AudioAnalog/AnalogAudio.h"
// #include "AudioTools/AudioStreams.h"

typedef void (*StreamDelegate)(const char *type, const char *value);

class InternetRadio
{
public:
    InternetRadio();   

    inline void selectStation(const RadioStation& station)
    {
        _station = &station;

        if(_stream != NULL){
            _stream->open(_station->Url);
        }
    }

    inline bool muted() const
    {
        return _muted;
    }

    inline void setMute(bool muted)
    {
        if (_output == NULL) {
            return;
        }

        _muted = muted;

        if (_muted) {
            _output->SetGain(0);            
        } else {
            _output->SetGain(_gain);
        }
    }

    void Loop();

    void Play(int output = 0, int input = 0);

    void Stop();

    void SampleCallback(SampleDelegate delegate);
   
    static StreamDelegate StreamChanged;

protected:
    AudioOutput* createI2S(int port=0, float gain = 1, int output_mode=0, int dma_buf_count = 8, int use_apll=0) const {
        auto output = new AudioOutputI2S(port, output_mode, dma_buf_count, use_apll);
        output->SetPinout(26, 25, 33);
        output->SetGain(gain);
        return output;
    }

    AudioOutput* createSPDIF(int port=0, float gain = 1, int dout_pin=SPDIF_OUT_PIN_DEFAULT, int dma_buf_count = DMA_BUF_COUNT_DEFAULT) const {
        auto output = new AudioOutputSPDIF(dout_pin, port, dma_buf_count);
        output->SetGain(gain);
        return output;
    }

    // AudioStream* createADC() const {
    //     auto input = new AnalogAudioStream();
    //     input->begin(input->defaultConfig(RX_MODE));
    //     return input;
    // }

private:
    bool _muted = false;
    float _gain = 1;

    const RadioStation* _station;
    
    const int _bufferSize = 16 * 1024; // buffer in byte, default 16 * 1024 = 16kb
        
    AudioFileSourceICYStream* _stream;

    AudioGeneratorMP3* _mp3;
    
    AudioFileSourceBuffer* _buffer;

    AudioOutputWithCallback* _output;

    static void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string);
    static void StatusCallback(void *cbData, int code, const char *string);
};