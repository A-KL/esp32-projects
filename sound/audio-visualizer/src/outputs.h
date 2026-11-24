#pragma once

#ifndef I2S_COUNT
#define I2S_COUNT 1
#endif

#ifndef I2S_DEFAULT
#define I2S_DEFAULT 0
#endif

#ifdef I2S_MASTER
  #define CONFIG_OUTPUT_MASTER(config) ((config).is_master = I2S_MASTER)
#else
  #define CONFIG_OUTPUT_MASTER(config) ((config).is_master = true)
#endif

#ifdef I2S_MCLK_MULTIPLE
  #define CONFIG_OUTPUT_MCLK(config) ((config).mclk_multiple = I2S_MCLK_MULTIPLE)
#else
  #define CONFIG_OUTPUT_MCLK(config)
#endif

#ifdef I2S_MCK
  #define CONFIG_OUTPUT_MCK(config) ((config).pin_mck = I2S_MCK)
#else
  #define CONFIG_OUTPUT_MCK(config)
#endif

#ifdef I2S_IN_SD
  #define CONFIG_OUTPUT_RX(config) ((config).pin_data_rx = I2S_IN_SD)
#else
  #define CONFIG_OUTPUT_RX(config)
#endif

#ifdef I2S_BPS
  #define CONFIG_OUTPUT_BPS(config) ((config).bits_per_sample = I2S_BPS)
#else
  #define CONFIG_OUTPUT_BPS(config)
#endif

#ifdef I2S_SAMPLE_RATE
  #define CONFIG_OUTPUT_SAMPLE_RATE(config) ((config).sample_rate = I2S_SAMPLE_RATE)
#else
  #define CONFIG_OUTPUT_SAMPLE_RATE(config)
#endif

#define CONFIG_OUTPUT(config) \
  (config).pin_ws = I2S_WS; \
  (config).pin_bck = I2S_BCK; \
  (config).pin_data = I2S_SD; \
  CONFIG_OUTPUT_BPS(config); \
  CONFIG_OUTPUT_SAMPLE_RATE(config); \
  CONFIG_OUTPUT_MASTER(config); \
  CONFIG_OUTPUT_MCLK(config); \
  CONFIG_OUTPUT_MCK(config); \
  CONFIG_OUTPUT_RX(config); \

#define CONFIG_OUTPUTS(config, index) \
  (config).port_no = index; \
  (config).pin_ws = I2S_ ## index ## _ ## WS; \
  (config).pin_bck = I2S_ ## index ## _ ## BCK; \
  (config).pin_data = I2S_ ## index ## _ ## SD; \
  (config).is_master = I2S_ ## index ## _ ## MASTER; \

template <typename TConfig>
inline void output_config(TConfig& config, int index = 0)
{
#if defined ARDUINO_ARCH_ESP32
  if (index == 0) {
    #ifdef I2S_WS
      CONFIG_OUTPUT(config);
    #else
      CONFIG_OUTPUTS(config, 0);
    #endif
  }
#if defined(I2S_1_WS) and (defined(I2S_COUNT) && (I2S_COUNT > 1))
  else if (index == 1) {
    CONFIG_OUTPUTS(config, 1);
  }
#endif
#if defined(I2S_2_WS) and (defined(I2S_COUNT) && (I2S_COUNT > 2))
  else if (index == 2) {
    CONFIG_OUTPUTS(config, 2);
  }
#endif

#endif
}

template <typename TStream>
inline void output_streams_config(TStream* streams, int count)
{
  for (int i=0; i<count; i++)
  {
    auto config = streams[i].defaultConfig();
    output_config(config, i);
    config.logInfo();
    streams[i].begin(config);
  }
}

template <typename TStream>
inline void output_streams_config(TStream* streams, const AudioInfo& info, int count)
{
  for (int i=0; i<count; i++)
  {
    auto config = streams[i].defaultConfig();
    config.copyFrom(info);
    output_config(config, i);
    config.logInfo();
    streams[i].begin(config);
  }
}