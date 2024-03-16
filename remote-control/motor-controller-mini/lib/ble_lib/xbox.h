#pragma once

struct xbox_controller_data_t
{
    uint16_t joy_l_h; // bytes 0 1. 0 to 65535, middle 32768
    uint16_t joy_l_v; // bytes 2 3. 0 to 65535, middle 32768

    uint16_t joy_r_h; //  bytes 4 5. 0 to 65535, middle 32768
    uint16_t joy_r_v; //  bytes 6 7. 0 to 65535, middle 32768

    uint16_t trg_l; // bytes 8 9. 0 to 1023
    uint16_t trg_r; // bytes 10 11. 0 to 1023

    uint8_t dpad; // 12. 1 -> U, 2 -> UR, 3 -> R, 4 -> DR, 5 -> D, 6 -> DL, 7 -> L, 8 -> LU

    // uint8_t main_buttons; // 13
    bool btnA : 1;
    bool btnB : 1;
    bool _padding_1 : 1;
    bool btnX : 1;
    bool btnY : 1;
    bool _padding_2 : 1;
    bool btnL : 1;
    bool btnR : 1;

    // uint8_t secondary_buttons; // 14
    bool _padding_3 : 2;
    bool btnSelect : 1;
    bool btnStart : 1;
    bool btnXbox : 1;
    bool btnLStick : 1;
    bool btnRStick : 1;
    bool _padding_4 : 1;

    // uint8_t share_button; // 15
    bool btnShare : 1;
    bool _padding_5 : 7;
};