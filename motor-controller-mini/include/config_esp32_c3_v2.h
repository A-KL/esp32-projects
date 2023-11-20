#ifndef config_esp32_c3_h
#define config_esp32_c3_h

#include <types.h>

const int pwm_pins[] = { 0, 1 };

const int adc_pins[] = { 4, 3 };

const int sbus_rx_tx_pins[] = { 20, 21 };

const motor_drive_mode_t motor_modes[] = { motor_drive_mode_t::a_b_en };

const int motors_count = 2;

const motor_pins_t motor_pins[motors_count] = {
    { 7,  6, 5, 1, 2, 0 }, //a, b, en, pwm_a, pwm_b, pwm_en
    { 8, 10, 2, 4, 5, 3 }  //b, b, en, pwm_a, pwm_b, pwm_en
};

motor_config_t motors_config[motors_count] = 
{
    { motor_modes[0], false, pwm, 0 }, // a_b_en, false, sbus input, channel 1
    { motor_modes[0], false, pwm, 1 }  // a_b_en, false, sbus input, channel 2
};

#endif