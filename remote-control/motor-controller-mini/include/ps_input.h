#pragma once

#include <esp32-hal-log.h>
//#include <driver_limits.h>
#include <driver_config.h>
#include <inputs_queue.h>

#define INPUT_PS_RANGE      255
#define INPUT_PS_HALF_RANGE (INPUT_PS_RANGE/2)
#define INPUT_PS_MIN        0
#define INPUT_PS_MAX        (INPUT_PS_MIN + INPUT_PS_RANGE)

#define INPUT_PS_DEAD_ZONE  10

#ifdef HAS_BLUETOOTH

#include <Ps3Controller.h>

static queue_t<ps3_t> ps_input_queue;

void ps3_on_connect() 
{
    log_i("PS3 Controller Connected.");
    Ps3.setPlayer(1);
}

void ps3_on_disconnect() 
{
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

    Data_t data = { 
        Ps3.data.analog.stick.lx, 
        Ps3.data.analog.stick.ly,
        Ps3.data.analog.stick.rx,
        Ps3.data.analog.stick.ry,

        Ps3.data.analog.button.l2,
        Ps3.data.analog.button.r2,
        Ps3.data.analog.button.l1,
        Ps3.data.analog.button.r1,

        Ps3.data.analog.button.left,
        Ps3.data.analog.button.up,
        Ps3.data.analog.button.right,
        Ps3.data.analog.button.down,

        Ps3.data.analog.button.square,
        Ps3.data.analog.button.triangle,
        Ps3.data.analog.button.circle,
        Ps3.data.analog.button.cross 
    };

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

    queue_init(ps_input_queue);
#endif
}

uint8_t ps_receive(int16_t* outputs)
{
#ifdef HAS_BLUETOOTH
    if (!Ps3.isConnected()) 
    {
        return 0;
    }

    ps3_t data;

    if (queue_receive(ps_input_queue, data))
    {
        int16_t values[] = {
            data.analog.stick.lx, 
            data.analog.stick.ly,
            data.analog.stick.rx,
            data.analog.stick.ry,

            data.analog.button.l2,
            data.analog.button.r2,
            data.analog.button.l1,
            data.analog.button.r1,

            data.analog.button.left,
            data.analog.button.up,
            data.analog.button.right,
            data.analog.button.down,

            data.analog.button.square,
            data.analog.button.triangle,
            data.analog.button.circle,
            data.analog.button.cross 
        };

        // TODO: test this
        memcpy(outputs, values, sizeof(values));

        return sizeof(values) / sizeof(int16_t);
    }

    return false;

#elif
    return false;
#endif
}