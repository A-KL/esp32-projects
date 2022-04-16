#pragma once

class AdcAudioDevice
{
public: 
  AdcAudioDevice(float real[], float img[], int size = 512, uint f = 44100, int pin = 37) :
    _mutex(xSemaphoreCreateBinary()),
    _real_l(real),
    _img_l(img),
    _size(size),
    _freq(f),
    _pin(pin),
    _sampling_period_us(round(1000000 * (1.0 / _freq)))
  { }

  bool loop()
  {
    if (!lock())
    {
        return false;
    }

    unsigned long newTime;

    for (int i = 0; i < _size; i++)
    {
        newTime = micros();

        _real_l[i] = analogRead(_pin); // A conversion takes about 9.7uS on an ESP32
        _img_l[i] = 0;

        // vReal_r[i] = analogRead(AUDIO_IN_PIN); // A conversion takes about 9.7uS on an ESP32
        // vImag_r[i] = 0;

        while ((micros() - newTime) < _sampling_period_us) { /* chill */ }
    }

    return unlock();
  }

  inline bool lock() const
  {
      return (xSemaphoreTake(_mutex, 100) == pdTRUE);
  }

  inline bool unlock() const
  {
      return (xSemaphoreGive(_mutex) == pdTRUE);
  }

private:
  SemaphoreHandle_t _mutex;

  float* _real_l;
  float* _img_l;
  const int _size;
  const unsigned int _freq;
  const int _pin;

  const unsigned int _sampling_period_us;
};