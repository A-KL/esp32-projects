#define USE_HELIX 
#define SPDIF_DATA_PIN 27

#include "BluetoothA2DPSink.h"
#include "AudioTools.h"
#include "AudioCodecs/CodecMP3Helix.h"

template<typename T>
class VisualizerStream : public AudioPrint, public AudioBaseInfoDependent {   
    public:
        VisualizerStream(xQueueHandle handle, bool active=false) {
            this->queue_handle = handle;
            this->active = active;
        }

        void begin(){
            LOGD(LOG_METHOD);
            this->active = true;
        }

        /// Sets the CsvStream as inactive 
        void end() {
            LOGD(LOG_METHOD);
            active = false;
        }

        /// defines the number of channels
        virtual void setAudioInfo(AudioBaseInfo info) {
            LOGI(LOG_METHOD);
            info.logInfo();
            this->channels = info.channels;
        };

        /// Writes the data - formatted as CSV -  to the output stream
        virtual size_t write(const uint8_t* data, size_t len) {   
            if (!active) return 0;
            LOGD(LOG_METHOD);
            size_t lenChannels = len / (sizeof(T)*channels); 
            data_ptr = (T*)data;
            for (size_t j=0;j<lenChannels;j++){     
                if (queue_handle != nullptr && data_ptr != nullptr) {
                    lastFrame.left = (int16_t)*data_ptr++;
                    lastFrame.right = (int16_t)*data_ptr++;
                    xQueueSend(queue_handle, &lastFrame, 0);
                }
            }
            return len;
        }

    protected:
        AudioFrame lastFrame {0, 0};
        xQueueHandle queue_handle;
        T *data_ptr;
        int channels = 2;
        bool active = false;        
};

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
static MP3DecoderHelix mp3Decoder;
static EncodedAudioStream decoder;
static MultiOutput output(metadata, decoder); 
// StreamCopy copier;

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

    decoder.begin(target, &mp3Decoder);
    decoder.setNotifyAudioChange(*target);

    auto vis_output = new VisualizerStream<int16_t>(audioFrameQueue);
    output.add(*vis_output);
    vis_output->begin();

    auto copier = new StreamCopy();

    copier->begin(output, *source);

    return copier;
}

void loopAudio(StreamCopy* copier) {
   copier->copy();
}

