
#define INPUT_SBUS_MIN  200
#define INPUT_SBUS_MAX  1800

#define INPUT_PWM_MAX   2000
#define INPUT_PWM_MIN   1024
#define INPUT_ADC_MAX   4095
#define INPUT_ADC_MIN   1

#define MOTOR_FREQ      100
#define MOTOR_RES       10

#ifndef config_esp32_h
#define config_esp32_h

const int MAX_DUTY_CYCLE = (int)(pow(2, MOTOR_RES) - 1);

#endif