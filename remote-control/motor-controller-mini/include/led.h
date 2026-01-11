#pragma once

#include <arduino-timer.h>

#ifdef RGB_LED_PIN
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

        _rgb.setPixelColor(0, _rgb.Color(255, 0, 0));
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
        rgb->setPixelColor(0, rgb->Color(0, 255, 0));
      }
      rgb->show();

      return true;
    }
};

static auto rgb_led = Adafruit_NeoPixel(1, RGB_LED_PIN, NEO_RGB + NEO_KHZ800);

#define indicator_t(X) rgb_animation_t(rgb_led, X)

#endif

#ifdef LED_PIN
//template <typename TState = bool>
class led_t
{
  public:
    led_t(const int pin, const bool state_on = LOW) 
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
    led_animation_t(led_t& led, Timer<>& timer) 
      : _led(led), _timer(timer), _task(NULL) 
    { }

    void input_received() 
    {
      if (_task) {
        _timer.cancel(_task);
        _task = NULL;

        _led.on();
      }
    }

    void input_lost() {
       if (!_task) {
        _task = _timer.every(300, on_led_timer, this);
      }
    }

  private:
    led_t& _led;
    Timer<>& _timer;
    void* _task;

    static bool on_led_timer(void* arg) 
    {
      auto led_animation = (led_animation_t*)arg;
      led_animation->_led.toggle();

      return true;
    }
};

static auto led = led_t(LED_PIN);

#define indicator_t(X) led_animation_t(led, X)

#endif

#if not defined(LED_PIN) && not defined(RGB_LED_PIN)

class no_animation_t
{
  public:
    no_animation_t(Timer<>& timer)
    { }

    inline void input_received() {
    }

    inline void input_lost() {
    }
};

#define indicator_t(X) no_animation_t(X)

#endif

// LED_TYPE=RGB
// LED_TYPE=None
// LED_PIN=7