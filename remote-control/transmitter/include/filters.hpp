#pragma once

#include <Arduino.h>
#include <config_esp32.hpp>

#ifdef __cplusplus
extern "C" {
#endif

class InputFilter
{
  public:
    virtual int read() = 0;
};

class TwoWaySwitchFilter : public InputFilter
{  
  public:
    TwoWaySwitchFilter(const short pin) : _pin(pin)
    {
      pinMode(_pin, INPUT_PULLUP);
    }

    int read()
    {
      return digitalRead(_pin) == LOW ? ADC_MIN : ADC_MAX;
    }

  private:
    const short _pin;
};

class ThreeWaySwitchFilter : public InputFilter
{  
  public:
    ThreeWaySwitchFilter(const short pinA, const short pinB) : _pinA(pinA), _pinB(pinB)
    {
      pinMode(_pinA, INPUT_PULLUP);
      pinMode(_pinB, INPUT_PULLUP);
    }

    int read()
    {
      auto valueA = digitalRead(_pinA);
      auto valueB = digitalRead(_pinB);

      if (valueA == HIGH && valueB == LOW) {
        return ADC_MIN;
      }
      if (valueA == LOW && valueB == HIGH) {
        return ADC_MAX;
      }

      return (ADC_MAX - ADC_MIN) / 2;
    }

  private:
    const short _pinA, _pinB;
};

class SmoothingFilter : public InputFilter
{
  public:
    SmoothingFilter(const short channel, const int padding = 0, const int error = 4)
    {
      _channel = channel;
      _padding = padding;
      _error = error;
      
      for(auto i = 0; i < _size; i++)
      {
        _readings[i] = 0;
      }
    }
    
    int read()
    {
      _total -= _readings[_index];
      
      auto read = analogRead(_channel) + _padding;
      _readings[_index] = abs(_readings[_index] - read) > _error ? read : _readings[_index];
      
      _total += _readings[_index];
      
      _index++;
      
      if (_index >= _size) {
        _index = 0;
      }
      
      return _total / _size;
    }
    
  private:
    const static int _size = 10;
    int _error = 3;
    int _padding = 0;
    short _channel = 0;
    int _total = 0;
    int _index = 0;
    int _readings[_size];
};

#ifdef __cplusplus
}
#endif