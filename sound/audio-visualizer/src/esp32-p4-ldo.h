#pragma once 

#if defined(CONFIG_IDF_TARGET_ESP32P4) && __has_include("esp32-hal-ldo.h")
  #include "esp32-hal-ldo.h"

  #define AUDIO_PIN_USES_P4_VO4(pin) ((pin) >= 39 && (pin) <= 48)
  static void enableP4I2SVo4IfNeeded()
  {
    bool usesVo4 = false;
    #ifdef I2S_BCK
      usesVo4 |= AUDIO_PIN_USES_P4_VO4(I2S_BCK);
    #endif
    #ifdef I2S_WS
      usesVo4 |= AUDIO_PIN_USES_P4_VO4(I2S_WS);
    #endif
    #ifdef I2S_SD
      usesVo4 |= AUDIO_PIN_USES_P4_VO4(I2S_SD);
    #endif
    #ifdef I2S_MCK
      usesVo4 |= AUDIO_PIN_USES_P4_VO4(I2S_MCK);
    #endif
    #ifdef I2S_1_BCK
      usesVo4 |= AUDIO_PIN_USES_P4_VO4(I2S_1_BCK);
    #endif
    #ifdef I2S_1_WS
      usesVo4 |= AUDIO_PIN_USES_P4_VO4(I2S_1_WS);
    #endif
    #ifdef I2S_1_SD
      usesVo4 |= AUDIO_PIN_USES_P4_VO4(I2S_1_SD);
    #endif
    #ifdef I2S_2_BCK
      usesVo4 |= AUDIO_PIN_USES_P4_VO4(I2S_2_BCK);
    #endif
    #ifdef I2S_2_WS
      usesVo4 |= AUDIO_PIN_USES_P4_VO4(I2S_2_WS);
    #endif
    #ifdef I2S_2_SD
      usesVo4 |= AUDIO_PIN_USES_P4_VO4(I2S_2_SD);
    #endif

    if (!usesVo4) {
      return;
    }

    static ldo_channel_handle_t vo4Handle = nullptr;
    if (vo4Handle != nullptr) {
      return;
    }

    const esp_err_t err = ldoAcquireChannel(4, 3300, false, &vo4Handle);
    if (err != ESP_OK) {
      LOGE("Could not acquire P4 VO4 LDO for I2S GPIOs: %s", esp_err_to_name(err));
      return;
    }
    LOGI("Acquired P4 VO4 LDO channel 4 at 3300 mV for I2S");
  }
#endif