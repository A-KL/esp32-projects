#pragma once

#include <arduino-timer.h>

class led_t
{
  public:
    led_t(int led_pin, Timer<>& timer) 
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
    int _led_pin;
    Timer<>& _timer;
    void* _task;

    static bool on_led_timer(void* arg) 
    {
      auto led = (led_t*)arg;
      digitalWrite(led->_led_pin, !digitalRead(led->_led_pin));

      return true;
    }
};