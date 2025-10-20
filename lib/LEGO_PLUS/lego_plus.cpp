// #include "lego_plus.h"
// #include "CommUtil.h"

// #define SLAVE_ADDR        0x56
// #define MOTOR_ADDR_BASE   0x00
// #define ENCODER_ADDR_BASE 0x08
// #define FIRWMARE_VER_ADDR 0x64
// #define STEP_V            51

// CommUtil Util;

// /*************************************************
// Function:MotorRun
// Description: Motor forward and reverse API
// Input:
//       n:Motor 0 to motor 3
//   Speed:Speed value from -255 to +255,when speed=0,The motor stopped.
// Return: Successful return 1
// Others:
// *************************************************/
// int32_t MotorRun(uint8_t n, int16_t Speed) {
//     if (n > 3) return 0;

//     if (Speed <= -255) Speed = -255;

//     if (Speed >= 255) Speed = 255;

//     return Util.writeBytes(SLAVE_ADDR, MOTOR_ADDR_BASE + n * 2, (uint8_t *)&Speed, 2);
// }

// /*************************************************
// Function:ReadEncoder
// Description: Motor rotation code value
// Input:
//       n:Motor 0 to motor 3
// Return: Successful return 1
// Others:
// *************************************************/
// int32_t readEncoder(uint8_t n) {
//     uint8_t dest[4] = {0};

//     if (n > 3) return 0;

//     Util.readBytes(SLAVE_ADDR, ENCODER_ADDR_BASE + n * 4, 4, dest);

//     return *((int32_t *)dest);
// }

// uint8_t readVersion() {
//     uint8_t dest = 0;

//     Util.readBytes(SLAVE_ADDR, FIRWMARE_VER_ADDR, 1, &dest);

//     return dest;
// }
