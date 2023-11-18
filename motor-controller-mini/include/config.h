#ifndef config_h
#define config_h

#include <map>

typedef struct {
    motor_config_t motors[MOTORS_COUNT];

} controller_config;

// const std::map<InputType, int> channels = { 
//     { InputType::PWM, INPUT_PWM_CHANNELS },
//     { InputType::ADC, INPUT_ADC_CHANNELS }, 
//     { InputType::SBUS, INPUT_SBUS_CHANNELS }
// };

#endif