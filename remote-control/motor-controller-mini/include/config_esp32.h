
#pragma once

#define INPUT_ESP_NOW_MIN   0
#define INPUT_ESP_NOW_MAX   4095

#define INPUT_SBUS_MIN  180 //200
#define INPUT_SBUS_MAX  1800

#define INPUT_PWM_MAX   1996//2000
#define INPUT_PWM_MIN   970 //1024
#define INPUT_PWM_ZERO  800 //1020

#define INPUT_ADC_MAX   4095
#define INPUT_ADC_MIN   0

#define MOTOR_FREQ      100
#define MOTOR_RES       10
#define MOTOR_DUTY_CYCLE (long)(pow(2, MOTOR_RES) - 1)


#define SERVO_FREQ      50
#define SERVO_RES       14

// #define SERVO_LOW       500
// #define SERVO_HIGH      2500
//#define SERVO_DEFAULT     ((SERVO_LOW+SERVO_HIGH)/2)

#define SERVO_DUTY_CYCLE  ((long)((1<<SERVO_RES)-1))

#define SERVO_LOW       ((long)(SERVO_DUTY_CYCLE * 0.025))
#define SERVO_HIGH      ((long)(SERVO_DUTY_CYCLE * 0.125))

// #define SERVO_MIN_USEC (100* / SERVO_FREQ)
// #define SERVO_MAX_USEC (100* / SERVO_FREQ)
// #define SERVO_PERIOD_USEC (100* / SERVO_FREQ)

#define LEGO_SERVO_LOW  0
#define LEGO_SERVO_HIGH (long)(pow(2, SERVO_RES) - 1)