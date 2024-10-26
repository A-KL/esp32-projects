#pragma once

#include <sbus.h>

#include <driver_limits.h>
#include <driver_config.h>

bfs::SbusRx sbus_rx(sbus_serial, sbus_rx_tx_pins[0], sbus_rx_tx_pins[1], true);
bfs::SbusTx sbus_tx(sbus_serial, sbus_rx_tx_pins[0], sbus_rx_tx_pins[1], true);

extern bfs::SbusData sbus_data;

void sbus_init() {
    sbus_rx.Begin();
    sbus_tx.Begin();
}

bool sbus_loop() 
{
    if (!sbus_rx.Read()) {
        return false;
    }

    sbus_data = sbus_rx.data();

    // for (auto input_config : global_config["sbus"]) 
    // {
    //   auto sbus_value = constrain(sbus_data.ch[input_config.in_channel], INPUT_SBUS_MIN, INPUT_SBUS_MAX);
    //   switch (input_config.out_type)
    //   {
    //     case motor:
    //       outputs[input_config.out_channel] = map(sbus_value, INPUT_SBUS_MIN, INPUT_SBUS_MAX, -MOTOR_DUTY_CYCLE, MOTOR_DUTY_CYCLE);
    //       break;
    //     case servo:
    //       outputs_servo[input_config.out_channel] = map(sbus_value, INPUT_SBUS_MIN, INPUT_SBUS_MAX, SERVO_LOW, SERVO_HIGH);
    //       break;
    //     case servo_lego:
    //       outputs_servo_lego[input_config.out_channel] = map(sbus_value, INPUT_SBUS_MIN, INPUT_SBUS_MAX, LEGO_SERVO_LOW, LEGO_SERVO_HIGH);
    //       break;              
    //   }
    // }

    sbus_tx.data(sbus_data);
    sbus_tx.Write();

    return true;
}