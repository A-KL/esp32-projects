#pragma once

#include "AudioTools.h"

#define CONFIG_OUTPUT \
  config.pin_ws      = I2S_WS; \
  config.pin_bck     = I2S_BCK; \
  config.pin_data    = I2S_SD; \
  ## if defined(I2S_MASTER) \
    config.is_master   = I2S_MASTER; \ 
  ## else \
    config.is_master   = true; \
  ## endif \
  ## if defined(I2S_MCLK_MULTIPLE) \
    config.mclk_multiple = I2S_MCLK_MULTIPLE; \
  ## endif \
  ## if defined(I2S_MCK) \
    config.pin_mck     = I2S_MCK; \
  ## endif \
  ## if defined(I2S_IN_SD) \
    config.pin_data_rx = I2S_IN_SD; \
  ## endif \

#define CONFIG_OUTPUTS(i) \
  config.pin_ws      = I2S_##i##_WS; \
  config.pin_bck     = I2S_##i##_BCK; \
  config.pin_data    = I2S_##i##_SD; \
  ## if defined(I2S_##i##_MASTER) \
    config.is_master   = I2S_##i##_MASTER; \ 
  ## else \
    config.is_master   = true; \
  ## endif \
  ## if defined(I2S_##i##_MCLK_MULTIPLE) \
    config.mclk_multiple = I2S_##i##_MCLK_MULTIPLE; \
  ## endif \
  ## if defined(I2S_##i##_MCK) \
    config.pin_mck     = I2S_##i##_MCK; \
  ## endif \
  ## if defined(I2S_##i##_IN_SD) \
    config.pin_data_rx = I2S_##i##_IN_SD; \
  ## endif \

void output_config(AudioInfo* configs)
{
#ifdef ARDUINO_ARCH_ESP32
# if not defined(I2S_COUNT)
  CONFIG_OUTPUT
#elif (I2S_COUNT==1)
  CONFIG_OUTPUTS(0)
#elif (I2S_COUNT==2)
  CONFIG_OUTPUT
  CONFIG_OUTPUTS(0)
#endif

//   config.pin_ws      = I2S_WS;
//   config.pin_bck     = I2S_BCK;
//   config.pin_data    = I2S_SD;
// # if defined(I2S_MASTER)
//   config.is_master   = I2S_MASTER;
// # else
//   config.is_master   = true;
// # endif
// # if defined(I2S_MCLK_MULTIPLE)
//   config.mclk_multiple = I2S_MCLK_MULTIPLE;
// # endif
// # if defined(I2S_MCK)
//   config.pin_mck     = I2S_MCK;
// # endif
// # if defined(I2S_IN_SD)
//   config.pin_data_rx = I2S_IN_SD;
// # endif
#endif
}

void output_speakers_config(AudioInfo& config)
{
#ifdef ARDUINO_ARCH_ESP32
  config.pin_ws      = I2S_WS;
  config.pin_bck     = I2S_BCK;
  config.pin_data    = I2S_SD;
# if defined(I2S_MASTER)
  config.is_master   = I2S_MASTER;
# else
  config.is_master   = true;
# endif
# if defined(I2S_MCLK_MULTIPLE)
  config.mclk_multiple = I2S_MCLK_MULTIPLE;
# endif
# if defined(I2S_MCK)
  config.pin_mck     = I2S_MCK;
# endif
# if defined(I2S_IN_SD)
  config.pin_data_rx = I2S_IN_SD;
# endif
#endif
}

void output_headphones_config(AudioInfo& config)
{
#ifdef ARDUINO_ARCH_ESP32
  config.pin_ws      = I2S_WS_H;
  config.pin_bck     = I2S_BCK_H;
  config.pin_data    = I2S_SD_H;
# if defined(I2S_MASTER_H)
  config.is_master   = I2S_MASTER_H;
# else
  config.is_master   = true;
# endif
# if defined(I2S_MCLK_MULTIPLE_H)
  config.mclk_multiple = I2S_MCLK_MULTIPLE_H;
# endif
# if defined(I2S_MCK_H)
  config.pin_mck     = I2S_MCK_H;
# endif
# if defined(I2S_IN_SD_H)
  config.pin_data_rx = I2S_IN_SD_H;
# endif
#endif
}