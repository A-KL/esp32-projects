#define USE_HELIX 

#include "AudioTools.h"
#include "AudioCodecs/CodecMP3Helix.h"

// Sources
//A2DPStream a2dp = A2DPStream::instance();
URLStream urlSource;
//ICYStream icy("","");

struct DacAudioOutputDevice
{
    const char* Name;
    const AudioStream* Device;
};

// Targets
I2SStream i2s;
SPDIFStream spdif;

// DacAudioOutputDevice Devices[] {
//     { "SPDIF",  &spdif },
//     { "AUX",  &i2s }
// };

// Generic
MetaDataPrint metadata;
MP3DecoderHelix mp3Decoder;
EncodedAudioStream decoder;
MultiOutput output(metadata, decoder); 
StreamCopy copier;

ConverterScaler<int16_t> volume(1.0, 0, 32767);

// Controls
AudioStream* source = NULL;
AudioStream* targetDevice = NULL;

void setupSpdif() {
    auto config = spdif.defaultConfig();
    config.pin_data = 27;
    config.sample_rate = 441000;
    config.channels = 2;
    config.bits_per_sample = 16;
    spdif.begin(config);

    targetDevice = &spdif;
}

void setupI2S() {
    auto config = i2s.defaultConfig(TX_MODE);
    config.pin_ws = 25;
    config.pin_bck = 26;
    config.pin_data = 33;
    i2s.begin(config);

    targetDevice = &i2s;
}

// callback for meta data
void printMetaData(MetaDataType type, const char* str, int len){
    Serial.print("==> ");
    Serial.print(toStr(type));
    Serial.print(": ");
    Serial.println(str);
}

AudioStream* setupWeb(const char* url) {
    urlSource.begin(url);

    metadata.setCallback(printMetaData);
    metadata.begin(urlSource.httpRequest());

    decoder.begin(targetDevice, &mp3Decoder);
    decoder.setNotifyAudioChange(*targetDevice);

    return &urlSource;
}

void setupAudio(const char* url) {

    AudioLogger::instance().begin(Serial, AudioLogger::Info); 

    setupSpdif();

    source = setupWeb(url);

    copier.begin(output, *source);
}

void loopAudio() {
    copier.copy(volume);
}

