#pragma once

#include "DriverDeviceInfo.h"

namespace audio_driver {

#ifndef I2S_SD_IN
#define I2S_SD_IN -1
#endif

#ifndef I2S_MCK
#define I2S_MCK -1
#endif

#ifndef I2S_0_SD_IN
#define I2S_0_SD_IN -1
#endif

#ifndef I2S_0_MCK
#define I2S_0_MCK -1
#endif

#ifndef I2S_1_SD_IN
#define I2S_1_SD_IN -1
#endif

#ifndef I2S_1_MCK
#define I2S_1_MCK -1
#endif

class StaticDriverPins : public DriverDeviceInfo {
 public:
  StaticDriverPins() {
  #if defined(I2C_SCL) && defined(I2C_SDA)
    // add i2c codec pins: scl, sda
    addI2C(PinFunction::CODEC, I2C_SCL, I2C_SDA);
  #endif

  #ifdef I2S_PA_PIN
    // speaker amplifier enable
    addPin(PinFunction::PA, I2S_PA_PIN, PinLogic::Output);
  #endif
  }

  bool configure(int port = 0) {
    if (index == 0) {
      #ifdef I2S_WS
      // add i2s pins: mclk, bck, ws, data_out, data_in
      addI2S(PinFunction::CODEC, I2S_MCK, I2S_BCK, I2S_WS, I2S_SD, I2S_SD_IN, port);
      #elifdef I2S_0_WS
        addI2S(PinFunction::CODEC, I2S_0_MCK, I2S_0_BCK, I2S_0_WS, I2S_0_SD, I2S_0_SD_IN, port);
      #endif
    } else if (port == 1) {
      #ifdef I2S_1_WS
        addI2S(PinFunction::CODEC, I2S_1_MCK, I2S_1_BCK, I2S_1_WS, I2S_1_SD, I2S_1_SD_IN, port);
      #endif
    } else if (port == 2) {
      #ifdef I2S_2_WS
        addI2S(PinFunction::CODEC, I2S_2_MCK, I2S_2_BCK, I2S_2_WS, I2S_2_SD, I2S_2_SD_IN, port);
      #endif
    } else {
    return false;
    }
    return true;
  }
};
}