#pragma once

#include <esp32-hal-log.h>
#include <driver_config.h>
#include <inputs_queue.h>

#ifdef HAS_BLE

#include <XboxSeriesXControllerESP32_asukiaaa.hpp>

static queue_t<int> xbox_input_queue;
// any xbox controller
static XboxSeriesXControllerESP32_asukiaaa::Core xbox_input;

#endif

void xbox_init() 
{
#ifdef HAS_BLE
    xbox_input.begin();
    queue_init(xbox_input_queue);
#endif
}

uint8_t xbox_receive(int16_t* outputs)
{
#ifdef HAS_BLE
    xbox_input.onLoop();

    if (!xbox_input.isConnected()) 
    {
        return false;
    }

    if (!xbox_input.isWaitingForFirstNotification())
    {
        log_d("XBOX Controller Address: %s", xbox_input.buildDeviceAddressStr());

        auto joystick_max = XboxControllerNotificationParser::maxJoy;

        auto d = xbox_input.xboxNotif.trigRT;

        return true;
    }
    return false;
#else
    return false;
#endif
}