#pragma once

#ifdef BT

#include <Ps3Controller.h>
#include <driver_limits.h>
#include <driver_config.h>

#define PS3_ADDRESS "01:02:03:04:05:06"

void on_connect() {
    log_i("PS3 Controller Connected.");
    Ps3.setPlayer(1);
}

void on_disconnect() {
  log_e("PS3 Controller connection lost.");
}

void on_data_received() {
    if( Ps3.data.button.cross ){
        Serial.println("Pressing the cross button");
    }

    if( Ps3.data.button.square ){
        Serial.println("Pressing the square button");
    }

    if( Ps3.data.button.triangle ){
        Serial.println("Pressing the triangle button");
    }

    if( Ps3.data.button.circle ){
        Serial.println("Pressing the circle button");
    }
}

void ps_init() {
    Ps3.attach(on_data_received);
    Ps3.attachOnConnect(on_connect);
    Ps3.attachOnDisconnect(on_disconnect);

    Ps3.begin(PS3_ADDRESS);
}

inline bool ps_loop() {
    return Ps3.isConnected();
}

#endif