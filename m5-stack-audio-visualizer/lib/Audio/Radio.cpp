#include "Radio.h"
#include "driver/i2s.h"

StreamDelegate InternetRadio::StreamChanged;

InternetRadio::InternetRadio()
        : _talkie(NULL), _mp3(NULL), _stream(NULL), _buffer(NULL), _outputAnalog(NULL), _outputSPDIF(NULL)
{ }

void InternetRadio::Loop()
{
    if (_mp3 == NULL)
    {
        return;
    }

    if (!_mp3->isRunning())
    {
        return;
    }

    if (!_mp3->loop())
    {
        _mp3->stop();
    } 
}

void InternetRadio::Play(const char* url = NULL) 
{
    _stream = new AudioFileSourceICYStream(url == NULL ? _url : url);
    _stream->RegisterMetadataCB(MDCallback, (void*)"ICY");

    _buffer = new AudioFileSourceBuffer(_stream, _bufferSize);
    _buffer->RegisterStatusCB(StatusCallback, (void*)"buffer");

    _outputSPDIF = new AudioOutputSPDIFWithCallback();

    // _outputAnalog = new CustomAudioOutputI2S(0, 0, 8, 1);
    // _outputAnalog->SetPinout(26, 25, 33);
    // _outputAnalog->SetGain(_gain*0.1);

    _mp3 = new AudioGeneratorMP3();
    _mp3->RegisterStatusCB(StatusCallback, (void*)"mp3");
    //_mp3->begin(_buffer, _outputAnalog);
    _mp3->begin(_buffer, _outputSPDIF);

    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0_CLK_OUT1);
    WRITE_PERI_REG(PIN_CTRL, 0xFFF0);
}

void InternetRadio::Stop() 
{
    if (_mp3) {
        _mp3->stop();
        delete _mp3;
        _mp3 = NULL;
    }

    if (_buffer) {
        _buffer->close();
        delete _buffer;
        _buffer = NULL;
    }

    if (_stream) {
        _stream->close();
        delete _stream;
        _stream = NULL;
    }

    if (_outputAnalog) {
        _outputAnalog->stop();
        delete _outputAnalog;
        _outputAnalog = NULL;
    }

    if (_outputSPDIF) {
        _outputSPDIF->stop();
        delete _outputSPDIF;
        _outputSPDIF = NULL;
    }   
}

void InternetRadio::SampleCallback(SampleDelegate delegate)
{
    if (_outputAnalog != NULL)
    {
        _outputAnalog->SampleCallback(delegate);
    }

    if (_outputSPDIF != NULL)
    {
        _outputSPDIF->SampleCallback(delegate);
    }    
}

void InternetRadio::MDCallback(void *cbData, const char *type, bool isUnicode, const char *string) 
{
    const char *ptr = reinterpret_cast<const char *>(cbData);

    (void) isUnicode; // Punt this ball for now

    // Note that the type and string may be in PROGMEM, so copy them to RAM for printf
    char s1[32], s2[64];

    strncpy_P(s1, type, sizeof(s1));
    s1[sizeof(s1) - 1] = 0;

    strncpy_P(s2, string, sizeof(s2));
    s2[sizeof(s2) - 1] = 0;

    Serial.printf("METADATA(%s) '%s' = '%s'\n", ptr, s1, s2);                                                                        
    Serial.flush();

    if (StreamChanged) {
        StreamChanged(s1, s2);
    }
}

void InternetRadio::StatusCallback(void *cbData, int code, const char *string) 
{
    const char *ptr = reinterpret_cast<const char *>(cbData);

    // Note that the string may be in PROGMEM, so copy it to RAM for printf
    char s1[64];
    strncpy_P(s1, string, sizeof(s1));
    s1[sizeof(s1) - 1] = 0;
    
    Serial.printf("STATUS(%s) '%d' = '%s'\n", ptr, code, s1);
    Serial.flush();
}

