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
static MetaDataPrint metadata;
static MP3DecoderHelix mp3;
static EncodedAudioStream decoder;
static MultiOutput output(metadata, decoder); 
static StreamCopy copier;
static xQueueAudioStream<int16_t> visualization_output(audioFrameQueue);

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

I2SStream* createI2S() {
    auto i2s = new I2SStream();
    auto config = i2s->defaultConfig(TX_MODE);
    config.pin_ws = 25;
    config.pin_bck = 26;
    config.pin_data = 33;
    //config.pin_mck = 10;
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

StreamCopy* setupAudio() {
    //AudioLogger::instance().begin(Serial, AudioLogger::Info);
    if (target != NULL) {
        delete target;
    }

    target = createSPDIF();

    if (source != NULL) {
        delete source;
    }

    source = createWeb(Stations[2].Url);

    visualization_output.begin(target);

    decoder.begin(&visualization_output, &mp3);
    decoder.setNotifyAudioChange(visualization_output);

    copier.begin(output, *source);

    return &copier;
}

void loopAudio(StreamCopy* copier) {
   copier->copy(volume);
}

