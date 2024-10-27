#pragma once

#include <esp32-hal-log.h>
#include <driver_limits.h>
#include <driver_config.h>
#include <inputs_queue.h>

#define INPUT_PS_RANGE      255
#define INPUT_PS_HALF_RANGE (INPUT_PS_RANGE/2)
#define INPUT_PS_MIN        0
#define INPUT_PS_MAX        (INPUT_PS_MIN + INPUT_PS_RANGE)

#define INPUT_PS_DEAD_ZONE  10

#ifdef HAS_BLUETOOTH

#include <Ps3Controller.h>

void ps3_on_connect() {
    log_i("PS3 Controller Connected.");
    Ps3.setPlayer(1);
}

void ps3_on_disconnect() {
  log_e("PS3 Controller connection lost.");
}

void ps3_on_data_received() 
{
#ifdef INPUT_PS3_DEBUG
    log_d("CPU Core: (%d) ANALOG: Left (%d,%d) Right (%d,%d) TRIGGER: Left (%d) Right (%d)", 
        xPortGetCoreID(),

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

inline void ps_init() 
{
#ifdef HAS_BLUETOOTH
    Ps3.attach(ps3_on_data_received);
    Ps3.attachOnConnect(ps3_on_connect);
    Ps3.attachOnDisconnect(ps3_on_disconnect);

    if (Ps3.begin(ps_controller_mac)) {
        log_i("Ps3 initialization...OK");
    }   
    else {
        log_w("Ps3 initialization...FAIL");
    }
#endif
}

uint8_t ps_receive(int16_t* outputs)
{
#ifdef HAS_BLUETOOTH
    if (!Ps3.isConnected()) 
    {
        return 0;
    }

    Data_t data;

    if (queue_receive(data))
    {
        auto channels = sizeof (data.values) / sizeof(int16_t);
        for (auto i=0; i<channels; i++)
        {
            outputs[i] = data.values[i];
        }
        return channels;
    }

    return false;

#elif
    return false;
#endif
}