#include <Arduino.h>
#include <NimBLEDevice.h>
#include <xbox.h>
#include "./bleretro32/bleretro32.h"

bool xbox_to_retro(uint8_t *bt_data, size_t bt_data_length, retro_joystick_status_t *retro_status)
{
    if (bt_data_length == sizeof(xbox_controller_data_t))
    {
        // auto xbox = (xbox_controller_data_t *)bt_data;
        // retro_status->btnA = xbox->btnA;
        // retro_status->btnA_alt = xbox->btnX;
        // retro_status->btnB = xbox->btnB;
        // retro_status->btnB_alt = xbox->btnY;

        // retro_status->up = xbox->dpad == DPAD_U || xbox->dpad == DPAD_UR || xbox->dpad == DPAD_UL;
        // retro_status->right = xbox->dpad == DPAD_UR || xbox->dpad == DPAD_R || xbox->dpad == DPAD_DR;
        // retro_status->down = xbox->dpad == DPAD_DR || xbox->dpad == DPAD_D || xbox->dpad == DPAD_DL;
        // retro_status->left = xbox->dpad == DPAD_DL || xbox->dpad == DPAD_L || xbox->dpad == DPAD_UL;

        // retro_status->up = retro_status->up || xbox->joy_l_v < AXIS_TOLERANCE;
        // retro_status->right = retro_status->right || xbox->joy_l_h > (AXIS_MAX - AXIS_TOLERANCE);
        // retro_status->down = retro_status->down || xbox->joy_l_v > (AXIS_MAX - AXIS_TOLERANCE);
        // retro_status->left = retro_status->left || xbox->joy_l_h < AXIS_TOLERANCE;

        // retro_status->auto_inc = xbox->btnR;
        // retro_status->auto_dec = xbox->btnL;

        return true;
    }
    else
    {
        return false;
    }
}

pad_definition_t pad_list[] = {
    {"Xbox Wireless Controller", xbox_to_retro}
};

void pad_init() {
    BLERetro32_Setup(pad_list, sizeof(pad_list) / sizeof(pad_definition_t));
}

void pad_loop() {
    auto cnn_status = BLERetro32_Loop();
}