#pragma once

#include <math.h>
#include <types.h>
#include <Arduino.h>

#ifndef MOTOR_INPUT_DEAD_ZONE
#define MOTOR_INPUT_DEAD_ZONE 10
#endif

#ifndef MOTOR_PWM_FQC
#define MOTOR_PWM_FQC         2500 // Hz
#endif

#ifndef MOTOR_PWM_RESOLUTION
#define MOTOR_PWM_RESOLUTION  8 // Bit
#endif

#ifndef INPUT_MOTOR_MAX_LIMIT
#define INPUT_MOTOR_MAX_LIMIT 1
#endif

const static int motor_duty_cycle = (255 * INPUT_MOTOR_MAX_LIMIT); // with 8 (0-255), 12 (0-4095), or 16 (0-65535) bit resolution

class Motor
{
public:
  Motor(const motor_config_t& config) 
  : _pin_a(config.pins.a), _pin_b(config.pins.b), _pin_en(config.pins.en), _channel(config.pins.en_channel), _mode(config.mode)
  {}

  Motor(const int8_t pin_a, const int8_t pin_b, const int8_t pin_en, const int8_t channel) 
  : _pin_a(pin_a), _pin_b(pin_b), _pin_en(pin_en), _channel(channel), _mode(a_b_en)
  {}

  Motor(const int8_t dir, const int8_t pin_en, const int8_t channel) 
  : _pin_a(dir), _pin_b(0), _pin_en(pin_en), _channel(channel), _mode(dir_en)
  {}

  Motor(const int8_t pin_a, const int8_t pin_b, const int8_t channel) 
  : _pin_a(pin_a), _pin_b(pin_b), _pin_en(0), _channel(channel), _mode(a_b)
  {}

  void init()
  {
    ledcSetup(_channel, MOTOR_PWM_FQC, MOTOR_PWM_RESOLUTION);

     switch (_mode)
     {
      case a_b_en:
          pinMode(_pin_a, OUTPUT);
          pinMode(_pin_b, OUTPUT);
          ledcAttachPin(_pin_en, _channel);
          break;

      case a_b:
          pinMode(_pin_a, OUTPUT);
          pinMode(_pin_b, OUTPUT);
          _attached_pin = -1;
          break;

      case dir_en:
          pinMode(_pin_a, OUTPUT);
          pinMode(_pin_en, OUTPUT);
          break;
     
      default:
          break;
     }
}

  void write(const int16_t speed)
  {
      switch (_mode)
      {
        case a_b_en:
          write_a_b_en(speed);
          break;

        case a_b:
          write_a_b(speed);
          break;

        case dir_en:
          write_dir_en(speed);
          break;
        
        default:
          break;
      }
  }

  template<short TMin, short TMax>
  inline void write(const int16_t speed)
  {
      write(config, map(constrain(speed, TMin, TMax), TMin, TMax, -motor_duty_cycle, motor_duty_cycle));
  }

private:
  const uint8_t _pin_a;
  const uint8_t _pin_b;
  const uint8_t _pin_en;
  const uint8_t _channel;
  const dc_mode_t _mode;
  int16_t _attached_pin = -1;

  inline bool near_zero(const int16_t value) {
    return (abs(value) < MOTOR_INPUT_DEAD_ZONE);
  }

  void write_a_b_en(const int16_t speed) 
  {
    if (near_zero(speed)) {
      digitalWrite(_pin_a, LOW);
      digitalWrite(_pin_b, LOW);

      ledcWrite(_channel, 0);
    }
    else {
      digitalWrite(_pin_a, speed > 0 ? HIGH : LOW);
      digitalWrite(_pin_b, speed > 0 ? LOW : HIGH);

      ledcWrite(_channel, abs(speed));
    }
  }

  void write_dir_en(const int16_t speed) 
  {
    if (near_zero(speed)) {
      ledcWrite(_channel, 0);
    }
    else {
      digitalWrite(_pin_a, speed > 0 ? HIGH : LOW);
      ledcWrite(_channel, abs(speed));
    }
  }

  void write_a_b(const int16_t speed) 
  {
    if (near_zero(speed)) 
    {
      if (_attached_pin != -1) 
      {
        ledcDetachPin(_attached_pin);
        pinMode(_attached_pin, OUTPUT);
        _attached_pin = -1;
      }
      digitalWrite(_pin_a, LOW);
      digitalWrite(_pin_b, LOW);

      return;
    }

    auto absolute_speed = abs(speed);

    if (speed > 0 && _attached_pin == _pin_b)
    {
        ledcDetachPin(_attached_pin);
        pinMode(_attached_pin, OUTPUT);
        digitalWrite(_attached_pin, LOW);

        _attached_pin = _pin_a;
        ledcAttachPin(_attached_pin, _channel);
    } 
    else if (speed < 0 && _attached_pin == _pin_a)
    {
        ledcDetachPin(_attached_pin);
        pinMode(_attached_pin, OUTPUT);
        digitalWrite(_attached_pin, LOW);

        _attached_pin = _pin_b;
        ledcAttachPin(_attached_pin, _channel);
    }
    ledcWrite(_channel, absolute_speed);
};
};