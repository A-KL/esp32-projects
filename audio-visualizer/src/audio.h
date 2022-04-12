#define USE_HELIX 
#define SPDIF_DATA_PIN 27

#include "BluetoothA2DPSink.h"
#include "AudioTools.h"
#include "AudioCodecs/CodecMP3Helix.h"
#include "xQueueAudioStream.h"

// Sources
// URLStream urlSource;
// BluetoothA2DPSink a2dp_sink;

// struct DacAudioOutputDevice
// {
//     const char* Name;
//     const AudioStream* Device;
// };

// DacAudioOutputDevice Devices[] {
//     { "SPDIF",  &spdif },
//     { "AUX",  &i2s }
// };

// Generic
static xQueueAudioStream<int16_t> visualization_output(audioFrameQueue);
static MP3DecoderHelix mp3;
static EncodedAudioStream decoder(visualization_output, mp3);
static MetaDataPrint metadata;
static MultiOutput output(metadata, decoder);
static StreamCopy copier;

static ConverterAutoCenter<int16_t> converter;
static ConverterScaler<int16_t> volume(1.0, 0, 32767);

// Controls
AudioStream* source = NULL;
AudioStream* target = NULL;

SPDIFStream* createSPDIF(uint32_t sample_rate = 441000) {
    auto spdif = new SPDIFStream();
    auto config = spdif->defaultConfig();
    config.pin_data = 27;
    config.sample_rate = sample_rate;
    config.channels = 2;
    config.bits_per_sample = 16;
    spdif->begin(config);
    return spdif;
}

I2SStream* createI2S(uint32_t sample_rate = 441000) {
    auto i2s = new I2SStream();
    auto config = i2s->defaultConfig(TX_MODE);
    //config.port_no = 0;
    config.pin_ws = 25;
    config.pin_bck = 26;
    config.pin_data = 33;
    //config.pin_mck = 10;
    config.sample_rate = sample_rate;
    config.i2s_format = I2S_STD_FORMAT;
    i2s->begin(config);
    return i2s;
}

// callback for meta data
static void printMetaData(MetaDataType type, const char* str, int len){
    Serial.print("==> ");
    Serial.print(toStr(type));
    Serial.print(": ");
    Serial.println(str);

    label_track.setText(str);
}

AudioStream* createWeb(const char* url) {
    auto source = new URLStream();
    source->begin(url);

    metadata.setCallback(printMetaData);
    metadata.begin(source->httpRequest());

    return source;
}

AudioStream* createAdc(uint16_t sample_rate = 44100) {
    auto source = new AnalogAudioStream();

    auto config = source->defaultConfig(RX_MODE);
    config.sample_rate = sample_rate;
    config.setInputPin1(36);
    source->begin(config);

    return source;
}

StreamCopy* setupAudio(int dest = 0, int src = 0) {

    if (target != NULL) {
        delete target;
    }

    switch(dest)
    {
        case 0:
            target = createSPDIF();
            break;

        case 1:
            target = createI2S();
            break;
    }
 
    visualization_output.begin(target);

    decoder.setNotifyAudioChange(visualization_output);
    decoder.begin();

    if (source != NULL) {
        delete source;
    }

    switch(src)
    {
        case 0:
            source = createWeb(Stations[2].Url);
            break;

        case 1:
            source = createAdc();
            break;
    }

    copier.begin(output, *source);
    
    return &copier;
}

void loopAudio(StreamCopy* copier) {
   copier->copy(volume);
}

