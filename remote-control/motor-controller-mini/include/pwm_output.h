#pragma once

#define SERVO_DUTY_CYCLE  ((long)((1<<SERVO_RES)-1))
#define SERVO_LOW         ((long)(SERVO_DUTY_CYCLE * 0.025))
#define SERVO_HIGH        ((long)(SERVO_DUTY_CYCLE * 0.125))

inline void servos_init() {
    for (int i = 0; i < servos_count; i++) {
        assert(ledcSetup(i, SERVO_FREQ, SERVO_RES));
    }
}

inline void servos_start() {
    for (auto i = 0; i < servos_count; i++) {
        ledcAttachPin(servos_pins[i], i);
    }
}

inline void pwm_stop() {
    for (auto i = 0; i < servos_count; i++) {
        ledcDetachPin(servos_pins[i]);
    }
}

inline void pwm_write(const uint8_t channel, const int16_t value) {
    ledcWrite(channel, value);
}

template<short TMin, short TMax>
inline void pwm_write(const uint8_t channel, const int16_t value) {
    ledcWrite(channel, map(value, TMin, TMax, SERVO_LOW, SERVO_HIGH));
}

inline void pwm_write(const int16_t* values, const uint8_t channels) {
    for (auto i = 0; i < min(channels, servos_count); i++) {
         pwm_write(i, values[i]);
    }
}

template<short TMin, short TMax>
inline void pwm_write(const int16_t* values, const uint8_t channels) {
    for (auto i = 0; i < min(channels, servos_count); i++) {
         pwm_write<TMin, TMax>(i, values[i]);
    }
}