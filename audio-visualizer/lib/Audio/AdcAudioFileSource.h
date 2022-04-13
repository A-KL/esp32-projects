#ifndef _ADCAUDIOFILESOURCE_H
#define _ADCAUDIOFILESOURCE_H

#include <Arduino.h>
#include "AudioStatus.h"

#include <driver/i2s.h>
#include <driver/adc.h>

class AdcAudioFileSource : public AudioFileSource{
    public:
        AdcAudioFileSource(const i2s_port_t port = I2S_NUM_0, int samples = 1024, unint Frequency = 44100) :
            _port(port)
        {
            
        }

        virtual bool open(const char *filename) override
        { 
            (void)filename; return false; 

              i2s_adc_enable(I2S_NUM_0);
        }

        virtual bool close()
        { 
            return false;
        };

        virtual bool isOpen() 
        { 
            return false; 
        };
        
        virtual uint32_t getSize() 
        {
            return 0; 
        };

        virtual uint32_t read(void *data, uint32_t len) override
        {
            i2s_read(_port,
                    (void*)samples,
                    sizeof(samples),
                    &bytesRead,   // workaround This is the actual buffer size last half will be empty but why?
                    portMAX_DELAY); // no timeout
        }

    private:
        const i2s_port_t _port;
        int16_t samples[samples];

};

#endif