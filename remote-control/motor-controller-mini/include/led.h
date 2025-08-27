#pragma once

#include <arduino-timer.h>

#include <Adafruit_NeoPixel.h> 

class rgb_animation_t
{
  public:
    rgb_animation_t(Adafruit_NeoPixel& rgb, Timer<>& timer) 
      : _rgb(rgb), _timer(timer), _task(NULL) 
    {
      _rgb.begin();
      _rgb.setBrightness(100);
    }

    void input_received() 
    {
      if (_task) {
        _timer.cancel(_task);
        _task = NULL;

        _rgb.setPixelColor(0, _rgb.Color(0, 255, 0));
        _rgb.show();
      }
    }

    void input_lost() {
       if (!_task) {
        _task = _timer.every(300, on_led_timer, &_rgb);
      }
    }

    private:
      Adafruit_NeoPixel& _rgb;
      Timer<>& _timer;
      void* _task;

    static bool on_led_timer(void* arg) 
    {
      auto rgb = (Adafruit_NeoPixel*)arg;

      if (rgb->getPixelColor(0)) {
        rgb->clear();
      } else {
        rgb->setPixelColor(0, rgb->Color(255, 0, 0));
      }
      rgb->show();

      return true;
    }
};

// template <
//     typename TState = bool
// >
class led_t
{
  public:
    inline led_t(const int pin, const bool state_on = LOW) 
      : _pin(pin), _state_on(state_on) 
    {
      pinMode(_pin, OUTPUT);
    }

    inline void on() {
      digitalWrite(_pin, _state_on);
    }

    inline void off() {
      digitalWrite(_pin, !_state_on);
    }

    inline void toggle() {
      digitalWrite(_pin, !digitalRead(_pin));
    }

    inline bool state() {
      return digitalRead(_pin) == _state_on;
    }

    inline void state(bool state) {
      if (state) {
        on();
      } else {
        off();
      }
    }

  private:
    const int _pin;
    const bool _state_on;
};

class led_animation_t
{
  public:
    led_animation_t(const int led_pin, Timer<>& timer) 
      : _led_pin(led_pin), _timer(timer), _task(NULL) 
    {
      pinMode(led_pin, OUTPUT);
    }

    void input_received() 
    {
      if (_task) {
        _timer.cancel(_task);
        _task = NULL;

        digitalWrite(_led_pin, LOW);
      }
    }

    void input_lost() {
       if (!_task) {
        _task = _timer.every(300, on_led_timer, this);
      }
    }

  private:
    const int _led_pin;
    Timer<>& _timer;
    void* _task;

    static bool on_led_timer(void* arg) 
    {
      auto led = (led_animation_t*)arg;
      digitalWrite(led->_led_pin, !digitalRead(led->_led_pin));

      return true;
    }
};