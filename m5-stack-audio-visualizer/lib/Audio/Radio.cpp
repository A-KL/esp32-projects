#include "Radio.h"

StreamDelegate InternetRadio::StreamChanged;

InternetRadio::InternetRadio()
        : _talkie(NULL), _mp3(NULL), _stream(NULL), _buffer(NULL), _output(NULL)
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

    //_output= new CustomAudioOutputI2S(0, 0);
    _output = new AudioOutputSPDIFWithCallback();
    // _output->SetPinout(26, 25, 33);
    // _output->SetGain(_gain*0.1);

    // _output = new AudioOutputWithCallback(audio);
    // _output->SetPinout(26, 25, 33);
    // _output->SetGain(_gain*0.1);

    //_i2s = new AudioOutputSPDIF();
    //_output = new AudioOutputWithCallback(_i2s, NULL);

    _mp3 = new AudioGeneratorMP3();
    _mp3->RegisterStatusCB(StatusCallback, (void*)"mp3");
    _mp3->begin(_buffer, _output);
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

    if (_output) {
        _output->stop();
        delete _output;
        _output = NULL;
    }    
}

void InternetRadio::SampleCallback(SampleDelegate delegate)
{
    if (_output != NULL)
    {
        _output->SampleCallback(delegate);
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

