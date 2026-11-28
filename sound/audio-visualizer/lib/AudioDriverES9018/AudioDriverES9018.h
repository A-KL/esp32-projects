#pragma once

#include <AudioDriver.h>
#include "ES9018.h"

namespace audio_driver {

/**
 * @brief Driver API for ES9018K2M DAC
 * @author Phil Schatzmann
 * @copyright GPLv3
 */

class AudioDriverES9018Class : public AudioDriver {
 public:
  AudioDriverES9018Class(uint16_t deviceAddr = ES9018::ES9018_ADDR) {
    i2c_default_address = deviceAddr;
  }
  
  bool setMute(bool mute) { return es9018.setVoiceMute(mute) == RESULT_OK; }
  bool setVolume(int volume) {
    return es9018.setVoiceVolume(limitValue(volume, 0, 100)) ==
           RESULT_OK;
  }
  int getVolume() {
    int vol;
    es9018.getVoiceVolume(&vol);
    return vol;
  }

  bool setInputVolume(int volume) override {
    (void)volume;
    return false;
  }

  bool isInputVolumeSupported() { return false; }

  ///
  void setMasterClockSource(int source) { master_clock_source = source; }

  ES9018& driver() { return es9018; }

 protected:
  ES9018 es9018;
  int master_clock_source = -1;

  bool init(codec_config_t codec_cfg) {
    int mclk_src = master_clock_source;
    if (mclk_src == -1) {
      mclk_src = 0;  // = FROM_MCLK_PIN;
    }
    AD_LOGI("MCLK_SOURCE: %d", mclk_src);

    assert(getI2C() != nullptr);
    es9018.setWire(getI2C());
    es9018.setAddress(getI2CAddress());
    return es9018.init(&codec_cfg) == RESULT_OK;
  }
  bool deinit() { return es9018.deinit() == RESULT_OK; }

  bool controlState(codec_mode_t mode) {
    return es9018.ctrlStateActive(mode, true) == RESULT_OK;
  }
  bool configInterface(codec_mode_t mode, I2SDefinition iface) {
    return es9018.configI2S(mode, &iface) == RESULT_OK;
  }
};

static AudioDriverES9018Class AudioDriverES9018;

}  // namespace audio_driver