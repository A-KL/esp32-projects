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
    for (short i = 0; i < servos_count; i++) {
        ledcAttachPin(servos_pins[i], i);
    }
}

inline void pwm_stop() {
    for (short i = 0; i < servos_count; i++) {
        ledcDetachPin(servos_pins[i]);
    }
}

inline void pwm_write(const short channel, const int value) {
    ledcWrite(channel, value);
}

inline void pwm_write(const short values[], const short size) {
    for (short i = 0; i < min(size, servos_count); i++) {
         pwm_write(i, values[i]);
    }
}

template<short TMin, short TMax>
inline void pwm_write(const short channel, const int value) {
    ledcWrite(channel, map(value, TMin, TMax, SERVO_LOW, SERVO_HIGH));
}

template<short TMin, short TMax>
inline void pwm_write(const short values[], const short size) {
    for (short i = 0; i < min(size, servos_count); i++) {
         pwm_write<TMin, TMax>(i, values[i]);
    }
}