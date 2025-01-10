#pragma once

#include <Arduino.h>

class motor_driver_t
{
  public:
    motor_driver_t(const int16_t pin_direction, const int16_t pin_en, const uint64_t frequency = 10000, const uint16_t resolution = 10, const uint16_t dead_zone = 10)
    : _pin_direction(pin_direction), _pin_en(pin_en), _frequency(frequency),  _resolution(resolution), _dead_zone(dead_zone)
    {}

    motor_driver_t& init()
    {
      if (_pin_en_ch == -1) {
        _pin_en_ch = _channels++;
      }

      ledcSetup(_pin_en_ch, _frequency, _resolution);
      ledcAttachPin(_pin_en, _pin_en_ch);   

      pinMode(_pin_direction, OUTPUT);

      return *this;
    }

    motor_driver_t& add_brake(int16_t pin_brake)
    {
      _pin_brake = pin_brake;
      pinMode(_pin_brake, OUTPUT);

      return *this;
    }

    motor_driver_t& add_stop(int16_t pin_stop)
    {
      _pin_stop = pin_stop;
      pinMode(_pin_stop, OUTPUT);

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

    int16_t _pin_stop = -1;
    int16_t _pin_brake = -1;

    const int16_t _pin_direction;
    const int16_t _pin_en;

    int16_t _pin_en_ch = -1;

    const uint64_t _frequency;
    const uint16_t _resolution;
    const uint16_t _dead_zone;

    inline bool near_zero(const int value) {
      return (abs(value) < _dead_zone);
    }
};

uint8_t motor_driver_t::_channels = 0;

#ifndef MOTOR_INPUT_DEAD_ZONE
#define MOTOR_INPUT_DEAD_ZONE 10
#endif

#ifndef MOTOR_PWM_FQC
#define MOTOR_PWM_FQC         10000 // Hz
#endif

#ifndef MOTOR_PWM_RESOLUTION
#define MOTOR_PWM_RESOLUTION  8 // Bit
#endif

#ifndef INPUT_MOTOR_MAX_LIMIT
#define INPUT_MOTOR_MAX_LIMIT 1
#endif

const static int motor_duty_cycle = (255 * INPUT_MOTOR_MAX_LIMIT); // with 8 (0-255), 12 (0-4095), or 16 (0-65535) bit resolution

// Utils

inline bool near_zero(const int value) {
  return (abs(value) < MOTOR_INPUT_DEAD_ZONE);
}

// Run motors

void motor_write_a_en(const uint8_t pin_a, const uint8_t channel_en, const int16_t speed) 
{
  if (near_zero(speed)) {
    ledcWrite(channel_en, 0);
  }
  else {
    digitalWrite(pin_a, speed > 0 ? HIGH : LOW);
    ledcWrite(channel_en, abs(speed));
  }
}

template<short TMin, short TMax>
void motor_write_a_en(const uint8_t pin_a, const uint8_t channel_en, const int16_t speed)
{
  motor_write_a_en(pin_a, channel_en, map(constrain(speed, TMin, TMax), TMin, TMax, -motor_duty_cycle, motor_duty_cycle));
}

// Init motors

void motor_init_a_en(const uint8_t pin_a, const uint8_t pin_en, const uint8_t channel_en = 0)
{
  pinMode(pin_a, OUTPUT);

  ledcSetup(channel_en, MOTOR_PWM_FQC, MOTOR_PWM_RESOLUTION);
  ledcAttachPin(pin_en, channel_en);
}

void motors_init()
{
    for (auto i = 0; i < motors_count; i++) {
    motor_init_a_en(motor_dir[i], motor_speed[i], motor_pwm_ch[i]);
  }
}