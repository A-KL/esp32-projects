#pragma once

inline void pwm_init() {
    for (int i = 0; i < pwm_outputs_count; i++) {
        ledcSetup(i, SERVO_FREQ, SERVO_RES);
    }
}

inline void pwm_start() {
    for (short i = 0; i < pwm_outputs_count; i++) {
        ledcAttachPin(pwm_output_pins[i], i);
    }
}

inline void pwm_stop() {
    for (short i = 0; i < pwm_outputs_count; i++) {
        ledcDetachPin(pwm_output_pins[i]);
    }
}

template<short TMin, short TMax>
inline void pwm_write(const short values[], const short size) {
    for (short i = 0; i < min(size, pwm_outputs_count); i++) {
         ledcWrite(i, map(values[i], TMin, TMax, SERVO_LOW, SERVO_HIGH));
    }
}

template<short TMin, short TMax>
inline void pwm_write(const short channel, const int value) {
    ledcWrite(channel, map(value, TMin, TMax, SERVO_LOW, SERVO_HIGH));
}