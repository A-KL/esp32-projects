#pragma once

#include "Codecs/CodecConstants.h"
#include "DriverCommon.h"
#include "Platforms/API_I2C.h"

#include <assert.h>
#include <stdint.h>

namespace audio_driver {

class ES9018 {
 public:
  static constexpr int ES9018_ADDR = 0x48;
  static constexpr int ES9018_ADDR_ALT = 0x49;

  static constexpr uint8_t ES9018_SYSTEM_SETTINGS = 0x00;
  static constexpr uint8_t ES9018_INPUT_CONFIG = 0x01;
  static constexpr uint8_t ES9018_RESERVED_2 = 0x02;
  static constexpr uint8_t ES9018_RESERVED_3 = 0x03;
  static constexpr uint8_t ES9018_AUTOMUTE_TIME = 0x04;
  static constexpr uint8_t ES9018_AUTOMUTE_LEVEL = 0x05;
  static constexpr uint8_t ES9018_VOLUME_DEEMPH = 0x06;
  static constexpr uint8_t ES9018_GENERAL = 0x07;
  static constexpr uint8_t ES9018_GPIO_CONFIG = 0x08;
  static constexpr uint8_t ES9018_MASTER_MODE = 0x0A;
  static constexpr uint8_t ES9018_CHANNEL_MAPPING = 0x0B;
  static constexpr uint8_t ES9018_DPLL_ASRC = 0x0C;
  static constexpr uint8_t ES9018_THD_COMPENSATION = 0x0D;
  static constexpr uint8_t ES9018_SOFT_START = 0x0E;
  static constexpr uint8_t ES9018_VOLUME1 = 0x0F;
  static constexpr uint8_t ES9018_VOLUME2 = 0x10;
  static constexpr uint8_t ES9018_GPIO_OSF = 0x15;
  static constexpr uint8_t ES9018_CHIP_STATUS = 0x40;
  static constexpr uint8_t ES9018_GPIO_STATUS = 0x41;

  ES9018() = default;

  void setWire(i2c_bus_handle_t handle) { i2c_handle = handle; }

  void setAddress(int addr) {
    if (addr > 0) i2c_addr = addr;
  }

  /// @brief Initialize ES9018K2M for I2S playback
  error_t init(codec_config_t* codec_cfg) {
    if (i2c_handle == nullptr) return RESULT_FAIL;
    assert(codec_cfg != nullptr);

    error_t ret = RESULT_OK;

    // Reset the register set, then reapply required reserved-bit values.
    ret |= writeReg(ES9018_SYSTEM_SETTINGS, 0x01);
    ret |= writeReg(ES9018_SYSTEM_SETTINGS, 0x00);
    ret |= writeReg(ES9018_RESERVED_2, 0x18);
    ret |= writeReg(ES9018_RESERVED_3, 0x10);
    ret |= writeReg(ES9018_VOLUME_DEEMPH, 0x4A);
    ret |= writeReg(ES9018_GENERAL, 0x80);
    ret |= writeReg(ES9018_DPLL_ASRC, 0x5A);
    ret |= writeReg(ES9018_SOFT_START, 0x8A);
    ret |= writeReg(ES9018_MASTER_MODE, 0x05);

    ret |= configI2S(CODEC_MODE_DECODE, &codec_cfg->i2s);

    // Disabled automute, bypass de-emphasis, and normal 0 dB volumes.
    ret |= writeReg(ES9018_AUTOMUTE_TIME, 0x00);
    ret |= writeReg(ES9018_AUTOMUTE_LEVEL, 0x68);
    ret |= writeReg(ES9018_VOLUME1, 0x00);
    ret |= writeReg(ES9018_VOLUME2, 0x00);

    return ret;
  }

  error_t deinit() { return suspend(); }

  error_t configI2S(codec_mode_t mode, I2SDefinition* iface) {
    if (iface == nullptr) return RESULT_FAIL;
    error_t ret = RESULT_OK;
    ret |= setBitsPerSample(iface->bits);
    ret |= configFmt(*iface);
    return ret;
  }

  error_t ctrlStateActive(codec_mode_t mode, bool ctrl_state_active) {
    if (ctrl_state_active) return start(mode);
    return stop(mode);
  }

  /// Volume is 0-100 percent. ES9018 uses 0.5 dB attenuation steps.
  error_t setVoiceVolume(int volume) {
    if (i2c_handle == nullptr) return RESULT_FAIL;
    volume = limitVolume(volume);

    // 255 register steps span 127.5 dB (0.5 dB per step).
    uint8_t attenuation = static_cast<uint8_t>(
        ((100 - volume) * 255 + 50) / 100);

    error_t ret = writeReg(ES9018_VOLUME1, attenuation);
    ret |= writeReg(ES9018_VOLUME2, attenuation);
    return ret;
  }

  error_t getVoiceVolume(int* volume) {
    if (volume == nullptr || i2c_handle == nullptr) return RESULT_FAIL;
    int reg = readReg(ES9018_VOLUME1);
    if (reg < 0) {
      *volume = 0;
      return RESULT_FAIL;
    }

    int attenuation = (reg * 100 + 127) / 255;
    *volume = limitVolume(100 - attenuation);
    return RESULT_OK;
  }

  error_t setVoiceMute(bool enable) {
    if (i2c_handle == nullptr) return RESULT_FAIL;

    int reg = readReg(ES9018_GENERAL);
    if (reg < 0) return RESULT_FAIL;
    uint8_t general = static_cast<uint8_t>(reg);
    general &= static_cast<uint8_t>(~0x03);
    if (enable) general |= 0x03;
    return writeReg(ES9018_GENERAL, general);
  }

  error_t getVoiceMute(int* mute) {
    if (mute == nullptr || i2c_handle == nullptr) return RESULT_FAIL;
    int reg = readReg(ES9018_GENERAL);
    if (reg < 0) return RESULT_FAIL;
    *mute = ((static_cast<uint8_t>(reg) & 0x03) == 0x03) ? 1 : 0;
    return RESULT_OK;
  }

  bool isLocked() const {
    int status = readReg(ES9018_CHIP_STATUS);
    return status >= 0 && (status & 0x01) != 0;
  }

  bool isAutomuteActive() const {
    int status = readReg(ES9018_CHIP_STATUS);
    return status >= 0 && (status & 0x02) != 0;
  }

  void readAll() const {
    for (int reg = 0; reg <= 0x15; ++reg) {
      AD_LOGI("REG:%02x, %02x", reg, readReg(static_cast<uint8_t>(reg)));
    }
  }

  error_t writeReg(uint8_t reg_addr, uint8_t data) const {
    return i2c_bus_write_bytes(i2c_handle, i2c_addr, &reg_addr,
                               sizeof(reg_addr), &data, sizeof(data));
  }

  int readReg(uint8_t reg_addr) const {
    uint8_t data = 0;
    error_t result = i2c_bus_read_bytes(i2c_handle, i2c_addr, &reg_addr,
                                         sizeof(reg_addr), &data, sizeof(data));
    return result == RESULT_OK ? static_cast<int>(data) : RESULT_FAIL;
  }

 private:
  i2c_bus_handle_t i2c_handle = nullptr;
  int i2c_addr = ES9018_ADDR;

  static int limitVolume(int volume) {
    if (volume < 0) return 0;
    if (volume > 100) return 100;
    return volume;
  }

  error_t setBitsPerSample(sample_bits_t bits) {
    int reg = readReg(ES9018_INPUT_CONFIG);
    if (reg < 0) return RESULT_FAIL;

    uint8_t input = static_cast<uint8_t>(reg);
    input &= static_cast<uint8_t>(~0xC0);
    switch (bits) {
      case BIT_LENGTH_16BITS:
        break;
      case BIT_LENGTH_24BITS:
        input |= 0x40;
        break;
      case BIT_LENGTH_32BITS:
        input |= 0x80;
        break;
      default:
        input |= 0x80;
        break;
    }
    return writeReg(ES9018_INPUT_CONFIG, input);
  }

  error_t configFmt(I2SDefinition fmt) const {
    int reg = readReg(ES9018_INPUT_CONFIG);
    if (reg < 0) return RESULT_FAIL;

    uint8_t input = static_cast<uint8_t>(reg);
    input &= static_cast<uint8_t>(~0x30);
    switch (fmt.fmt) {
      case I2S_NORMAL:
        break;
      case I2S_LEFT:
        input |= 0x10;
        break;
      case I2S_RIGHT:
      case I2S_DSP:
      default:
        // ES9018 supports I2S and left-justified PCM; I2S is the safe default.
        break;
    }
    input &= static_cast<uint8_t>(~0x03);
    input |= 0x00;  // I2S input
    return writeReg(ES9018_INPUT_CONFIG, input);
  }

  error_t start(codec_mode_t mode) {
    if (mode == CODEC_MODE_ENCODE || mode == CODEC_MODE_LINE_IN) {
      AD_LOGW("ES9018K2M does not support ADC input");
      if (mode == CODEC_MODE_LINE_IN) return RESULT_FAIL;
    }
    // Start normal DAC operation and remove both channel mutes.
    error_t ret = setVoiceMute(false);
    int reg = readReg(ES9018_SOFT_START);
    if (reg < 0) return RESULT_FAIL;
    reg |= 0x80;
    ret |= writeReg(ES9018_SOFT_START, static_cast<uint8_t>(reg));
    return ret;
  }

  error_t stop(codec_mode_t mode) const {
    (void)mode;
    int reg = readReg(ES9018_SOFT_START);
    if (reg < 0) return RESULT_FAIL;
    reg &= ~0x80;
    return writeReg(ES9018_SOFT_START, static_cast<uint8_t>(reg));
  }

  error_t suspend() const {
    error_t ret = writeReg(ES9018_SOFT_START, 0x0A);
    return ret;
  }
};

} /* namespace audio_driver */