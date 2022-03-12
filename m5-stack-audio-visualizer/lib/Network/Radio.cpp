#include "Radio.h"

InternetRadio::InternetRadio()
        : _talkie(NULL), _mp3(NULL), _stream(NULL), _buffer(NULL), _output(NULL)
{ }

void InternetRadio::Init()
{
    _output = new AudioOutputI2S(0, 1); // Output to builtInDAC
    _output->SetOutputModeMono(true);
    _output->SetGain(_gain*0.05);
}

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

void InternetRadio::StartPlaying(const char* url) 
{
    _stream = new AudioFileSourceICYStream(url);
    _stream->RegisterMetadataCB(MDCallback, (void*)"ICY");

    _buffer = new AudioFileSourceBuffer(_stream, _bufferSize);
    _buffer->RegisterStatusCB(StatusCallback, (void*)"buffer");

    _output = new AudioOutputI2S(0, 1); // Output to builtInDAC
    _output->SetOutputModeMono(true);
    _output->SetGain(_gain*0.05);

    _mp3 = new AudioGeneratorMP3();
    _mp3->RegisterStatusCB(StatusCallback, (void*)"mp3");
    _mp3->begin(_buffer, _output);
}

void InternetRadio::StopPlaying() 
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