#pragma once

#include <esp32-hal-log.h>
#include <driver_limits.h>
#include <driver_config.h>

#define PS3_INPUT_DEBUG

#ifdef HAS_BLUETOOTH

#include <Ps3Controller.h>

#define PS_INPUT_DEAD_ZONE 10

void on_connect() {
    log_i("PS3 Controller Connected.");
    Ps3.setPlayer(1);
}

void on_disconnect() {
  log_e("PS3 Controller connection lost.");
}

void on_data_received() {
    
#ifdef PS3_INPUT_DEBUG
    log_d("ANALOG: Left (%d,%d) Right (%d,%d) TRIGGER: Left (%d) Right (%d)", 

        Ps3.data.analog.stick.lx,
        Ps3.data.analog.stick.ly,
        
        Ps3.data.analog.stick.rx,
        Ps3.data.analog.stick.ry,
        
        Ps3.data.analog.button.l2,
        Ps3.data.analog.button.r2);
#endif

    // if( Ps3.data.button.cross ){
    //     log_d("Pressing the cross button\r\n");
    // }

    // if( Ps3.data.button.square ){
    //     log_d("Pressing the square button\r\n");
    // }

    // if( Ps3.data.button.triangle ){
    //     log_d("Pressing the triangle button\r\n");
    // }

    // if( Ps3.data.button.circle ){
    //     log_d("Pressing the circle button\r\n");
    // }
}
#endif

void ps_init() {
#ifdef HAS_BLUETOOTH
    Ps3.attach(on_data_received);
    Ps3.attachOnConnect(on_connect);
    Ps3.attachOnDisconnect(on_disconnect);

    Ps3.begin(ps_controller_mac);
#endif
}

inline bool ps_loop() {
#ifdef HAS_BLUETOOTH
    return Ps3.isConnected();
#endif
}