#pragma once

#include <Arduino.h>

class motor_driver_t
{
  public:
    motor_driver_t(
      const int16_t pin_direction, 
      const int16_t pin_en, 
      const int16_t pin_brake,
      const int16_t pin_stop,
      const uint64_t frequency = 10000, 
      const uint16_t resolution = 10, 
      const uint16_t dead_zone = 10)
    : _pin_direction(pin_direction), 
      _pin_en(pin_en), 
      _pin_brake(pin_brake),
      _pin_stop(pin_stop),
      _frequency(frequency), 
      _resolution(resolution), 
      _dead_zone(dead_zone)
    {}

    motor_driver_t& init()
    {
      if (_pin_en_ch == -1) {
        _pin_en_ch = _channels++;
      }

      ledcSetup(_pin_en_ch, _frequency, _resolution);
      ledcAttachPin(_pin_en, _pin_en_ch);   

      pinMode(_pin_direction, OUTPUT);
      // pinMode(_pin_brake, OUTPUT);
      // pinMode(_pin_stop, OUTPUT);

      return *this;
    }

    void stop(bool stop = true)
    {
        _stop = stop;
        digitalWrite(_pin_stop, _stop);
    }

    void brake(bool brake = true) const
    {
        digitalWrite(_pin_brake, brake);
    }

    void write(int16_t speed) const
    {
        if (near_zero(speed)) 
        {
          ledcWrite(_pin_en_ch, 0);
        }
        else 
        {
          digitalWrite(_pin_direction, speed > 0 ? HIGH : LOW);
          ledcWrite(_pin_en_ch, abs(speed));
        }
    }

  private:
    bool _stop = false;

    static uint8_t _channels;

    const int16_t _pin_stop;
    const int16_t _pin_brake;

    const int16_t _pin_direction;
    const int16_t _pin_en;

    int16_t _pin_en_ch = -1;

    const uint64_t _frequency;
    const uint16_t _resolution;
    const uint16_t _dead_zone;

    inline bool near_zero(const int16_t value) const {
      return (abs(value) < _dead_zone);
    }
};

uint8_t motor_driver_t::_channels = 0;

// const static int motor_duty_cycle = (255 * INPUT_MOTOR_MAX_LIMIT); // with 8 (0-255), 12 (0-4095), or 16 (0-65535) bit resolution

// template<short TMin, short TMax>
// void motor_write_a_en(const uint8_t pin_a, const uint8_t channel_en, const int16_t speed)
// {
//   motor_write_a_en(pin_a, channel_en, map(constrain(speed, TMin, TMax), TMin, TMax, -motor_duty_cycle, motor_duty_cycle));
// }