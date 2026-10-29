#pragma once

#include <LovyanGFX.h>

// #else
// 	if (lgfx::v1::gpio_in(MENU_BUTTON) == 0) {
//     changeAudioInput();
//     lgfx::delay(100);
// 	}

//   if (lgfx::v1::gpio_in(MENU_PIN_A) == 0) {
//     changeAudioInput();
//     lgfx::delay(100);
// 	}

//   if (lgfx::v1::gpio_in(MENU_PIN_B) == 0) {
//     changeAudioInput();
//     lgfx::delay(100);
// 	}

//   if (lgfx::v1::gpio_in(VOLUME_BUTTON) == 0) {
//     switchMuted();
//     lgfx::delay(100);
// 	}
// #endif

class AiEsp32RotaryEncoder {
  public:
    AiEsp32RotaryEncoder(
      const uint8_t encoder_pin_a, 
      const uint8_t encoder_pin_b, 
      const uint8_t encoder_pin_button,
      const int16_t encoder_pin_vcc,
      const int16_t encoder_steps) 
      : _encoder_pin_a(encoder_pin_a), _encoder_pin_b(encoder_pin_b), _encoder_pin_button(encoder_pin_button)
    {}

    inline void begin() 
    { }

    inline void setup(void (*ISR_callback)(void))
    { }

    inline void setBoundaries(long min, long max, bool loop)
    {
      _min = min;
      _max = max;
      _loop = loop;
    }

    inline void setEncoderValue(long value)
    {
      _changed |= _value != value;
      _value = value;
    }

    inline bool encoderChanged()
    {
      readEncoder_ISR();
      return _changed;
    }

    inline long readEncoder()
    {
      return _value;
    }

    inline void disableAcceleration() {}
    inline void enable() { _enabled = true; }
    inline void disable() { _enabled = false; }

    inline bool isEncoderButtonClicked()
    {
      if (!_enabled){
        return false;
      }

      auto result = lgfx::v1::gpio_in(_encoder_pin_button) == 0;
      if (result) {
        lgfx::delay(50);
      }
      return result;
    }

    void IRAM_ATTR readEncoder_ISR() 
    {
      if (!_enabled){
        return;
      }

      auto updated = _value;

      if (lgfx::v1::gpio_in(_encoder_pin_a) == 0) 
      {
        updated++;
        lgfx::delay(50);
      } 
      else if (lgfx::v1::gpio_in(_encoder_pin_b) == 0) 
      {
        updated--;
        lgfx::delay(50);
      }
      else
      {
        _changed = false;
        return;
      }

      if (!_loop) 
      {
          auto res = constrain(updated, _min, _max);
          _changed = _value != res;
          _value = res;
          return;
      } 
      else if (updated > _max) 
      {
        _value = _min;
      } 
      else if (updated < _min)
      {
        _value = _max;
      }
      else
      {
        _value = updated;
      }

      _changed = true;
    }

    void IRAM_ATTR readButton_ISR() {}

  private:
    const uint8_t _encoder_pin_a;
    const uint8_t _encoder_pin_b;
    const uint8_t _encoder_pin_button;

    long _value = 0;
    long _min = 0;
    long _max = 100;
    bool _loop = false;
    bool _changed = false;
    bool _enabled = true;
};
