#include "AudioTools.h"

template<typename T>
class xQueueAudioStream : public AudioStream
{
public:
    xQueueAudioStream(xQueueHandle handle, bool active=false) {
        this->queue_handle = handle;
        this->active = active;
        this->innerStream = nullptr;
    }

    virtual void setAudioInfo(AudioBaseInfo info) {
        LOGI(LOG_METHOD);
        info.logInfo();
        this->channels = info.channels;

        if (this->innerStream != nullptr){
            this->innerStream->setAudioInfo(info);
        }
    };

    virtual size_t write(const uint8_t* buffer, size_t size) {   
        if (!active) return 0;
        LOGD(LOG_METHOD);

        if (this->innerStream != nullptr){
            this->innerStream->write(buffer, size);
        }

        size_t lenChannels = size / (sizeof(T)*channels); 
        data_ptr = (T*)buffer;

        for (size_t j=0;j<lenChannels;j++){     
            if (queue_handle != nullptr && data_ptr != nullptr) {
                xQueueSend(queue_handle, data_ptr, 0);
                data_ptr+=channels;
            }
        }

        return size;
    }

    virtual size_t readBytes(uint8_t *buffer, size_t length) override { 
        if (this->innerStream != nullptr){
            return this->innerStream->readBytes(buffer, length);
        }
        return -1;
    }

    bool begin(AudioStream* inner = nullptr){
        LOGD(LOG_METHOD);
        this->innerStream = inner;
        this->active = true;

        return AudioStream::begin();
    }

    void begin(AudioStream& inner) {
        LOGD(LOG_METHOD);
        return this->begin(&inner);
    }

    void end() {
        LOGD(LOG_METHOD);
        active = false;
    }

    virtual size_t write(uint8_t data){
        if (this->innerStream != nullptr){
          //  return this->innerStream->write(data);
        } 
        return 0;
    }

    virtual int read() {
        if (this->innerStream != nullptr){
            return this->innerStream->read();
        }          
        return -1;
    }  

    virtual int peek() {
        if (this->innerStream != nullptr){
            return this->innerStream->peek();
        }           
        return -1;
    }  

    /// Provides the available audio data
    virtual int available() override {
        if (this->innerStream != nullptr){
            return this->innerStream->available();
        }            
        return -1;
    }

    /// Provides the available audio data
    virtual int availableForWrite() override {
        if (this->innerStream != nullptr){
            return this->innerStream->availableForWrite();
        }            
        return -1;        
    }

    void flush() override {
        if (this->innerStream != nullptr){
            return this->innerStream->flush();
        }              
    }

protected:
    AudioStream* innerStream;
    //AudioFrame lastFrame {0, 0};
    xQueueHandle queue_handle;
    T *data_ptr;
    int channels = 2;
    bool active = false;
};

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
                    lastFrame.left = *data_ptr; data_ptr++;
                    lastFrame.right = *data_ptr; data_ptr++;
                    xQueueSend(queue_handle, &lastFrame, 0);
                }
            }
            return len;
        }

        /// not supported
        virtual size_t write(uint8_t){
            return 0;
        }
        

    protected:
        AudioFrame lastFrame {0, 0};
        xQueueHandle queue_handle;
        T *data_ptr;
        int channels = 2;
        bool active = false;        
};
