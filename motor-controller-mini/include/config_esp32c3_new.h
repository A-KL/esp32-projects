#ifndef test_h
#define test_h

#include <types.h>

const int pwm_pins[] = { 0, 1 };

const int adc_pins[] = { 4, 3 };

const int sbus_tx_rx_pins[] = { 20, 21 };

const int motor_modes[] = { motor_drive_mode_t::a_b_en };

const motor_pins_t motor_pins[2] = {
    { 7,  6, 5 }, //a, b, en
    { 8, 10, 2 }  //b, b, en
};

#endif