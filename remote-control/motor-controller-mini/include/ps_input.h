#pragma once

#include <esp32-hal-log.h>
#include <driver_limits.h>
#include <driver_config.h>

#include <inputs_queue.h>

#define INPUT_PS_MIN       0
#define INPUT_PS_MAX       255
#define INPUT_PS_DEAD_ZONE 10
#define INPUT_PS3_DEBUG // temporary

#ifdef HAS_BLUETOOTH

#include <Ps3Controller.h>

void on_connect() {
    log_i("PS3 Controller Connected.");
    Ps3.setPlayer(1);
}

void on_disconnect() {
  log_e("PS3 Controller connection lost.");
}

void on_data_received() 
{
#ifdef INPUT_PS3_DEBUG
    log_d("ANALOG: Left (%d,%d) Right (%d,%d) TRIGGER: Left (%d) Right (%d)", 

        Ps3.data.analog.stick.lx,
        Ps3.data.analog.stick.ly,
        
        Ps3.data.analog.stick.rx,
        Ps3.data.analog.stick.ry,
        
        Ps3.data.analog.button.l2,
        Ps3.data.analog.button.r2);
#endif
    Data_t data;

    data.values[0] = Ps3.data.analog.stick.lx;
    data.values[1] = Ps3.data.analog.stick.ly;
    data.values[2] = Ps3.data.analog.stick.rx;
    data.values[3] = Ps3.data.analog.stick.ry;
    data.values[4] = Ps3.data.analog.button.l2;
    data.values[5] = Ps3.data.analog.button.r2;

    queue_send(data);
}
#endif

void ps_init() {
#ifdef HAS_BLUETOOTH
    Ps3.attach(on_data_received);
    Ps3.attachOnConnect(on_connect);
    Ps3.attachOnDisconnect(on_disconnect);

    if (Ps3.begin(ps_controller_mac)) {
        log_i("Ps3 initialization...\tOK");
    }   
    else {
        log_w("Ps3 initialization...\tFAIL");
    }
#endif
}

inline bool ps_loop() {
#ifdef HAS_BLUETOOTH
    return Ps3.isConnected();
#endif
}