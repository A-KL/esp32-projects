#pragma once

template<uint8_t PWM_CHANNELS_COUNT>
class EspArduinoHalDriver
{
protected:
    inline void init_pin(uint8_t pin, uint8_t direction) {
        pinMode(pin, direction);
    }

    inline void init_pwm_pin(uint8_t pin, uint8_t& channel = -1) {
        if (channel == -1)
        {
            channel = get_or_assign_channel(pin);
        }
        ledcAttachPin(pin, channel);
    }

    inline void write(uint8_t pin, int value, uint8_t& channel = -1) {
        if (channel == -1)
        {
            channel = get_or_assign_channel(pin);
        }
        ledcWrite(channel, value);
    }

    inline void write(uint8_t pin, bool value) {
        digitalWrite(pin, value);
    }



private:
    static uint8_t pwm_channels[PWM_CHANNELS_COUNT];

    int find_channel(uint8_t pin) {
        for (auto i=0; i<PWM_CHANNELS_COUNT; ++i) {
            if (pwm_channels[i] == pin) {
                return i;
            }
        }
    }

    int get_or_assign_channel(uint8_t pin) {
        for (auto i=0; i<PWM_CHANNELS_COUNT; ++i) {
            if (pwm_channels[i] == pin) {
                return i;
            }
        }
    }
};
