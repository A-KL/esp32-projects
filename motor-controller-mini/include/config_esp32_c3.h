#define LED_PIN       8

#define INPUT_0_PWM   0
#define INPUT_1_PWM   1

#define INPUT_0_ADC   4
#define INPUT_1_ADC   3

#define INPUT_RX_SBUS 20
#define INPUT_TX_SBUS 21

#define MOTORS_COUNT 2

#define MOTOR_0_OUTPUT_A  7
#define MOTOR_0_OUTPUT_B  6
#define MOTOR_0_OUTPUT_EN 5

#define MOTOR_1_OUTPUT_A  8
#define MOTOR_1_OUTPUT_B  10
#define MOTOR_1_OUTPUT_EN 2

#define MOTORS_DRIVER_MODES MotorDriverMode::A_B_EN
#define CONTROL_MODE 2

#define MOTOR_0_SBUS_CHANNEL  1
#define MOTOR_1_SBUS_CHANNEL  2

#define INPUT_SBUS_CHANNELS 8
#define INPUT_PWM_CHANNELS MOTORS_COUNT
#define INPUT_ADC_CHANNELS MOTORS_COUNT

const motor_config_t motors[MOTORS_COUNT] = 
{
    { a_b_en, false, sbus, 2 }, 
    { a_b_en, false, sbus, 4 }
};