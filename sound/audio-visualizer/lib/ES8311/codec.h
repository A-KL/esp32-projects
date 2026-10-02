#pragma once
#include "esp_check.h"
#include "es8311.h"

constexpr int16_t PLATFORM_AUDIO_PA = 53;
constexpr int16_t PLATFORM_ES8311_ADDRESS = 0x18;
constexpr int32_t PLATFORM_I2C_PORT = 0;
constexpr float PLATFORM_INIT_VOLUME = 70;

static es8311_handle_t codec = nullptr;

static esp_err_t codec_init(int32_t sample_rate = 44100) 
{
  pinMode(PLATFORM_AUDIO_PA, OUTPUT);
  digitalWrite(PLATFORM_AUDIO_PA, HIGH);

  codec = es8311_create(PLATFORM_I2C_PORT, PLATFORM_ES8311_ADDRESS);

  ESP_RETURN_ON_FALSE(codec, ESP_FAIL, "ES8311", "create failed");

  const es8311_clock_config_t clock = {
    .mclk_inverted = false,
    .sclk_inverted = false,
    .mclk_from_mclk_pin = true,
    .mclk_frequency = sample_rate * I2S_MCLK_MULTIPLE,
    .sample_frequency = sample_rate,
  };
  ESP_RETURN_ON_ERROR(es8311_init(codec, &clock, ES8311_RESOLUTION_16, ES8311_RESOLUTION_16), "ES8311", "init failed");

  ESP_RETURN_ON_ERROR(es8311_sample_frequency_config(codec, clock.mclk_frequency, clock.sample_frequency), "ES8311", "sample frequency failed");
  ESP_RETURN_ON_ERROR(es8311_microphone_config(codec, false), "ES8311", "disable microphone failed");
  
  return es8311_voice_volume_set(codec, PLATFORM_INIT_VOLUME, nullptr);
}

static esp_err_t codec_mute(bool mute)
{
  return es8311_voice_mute(codec, mute);
}

static esp_err_t codec_volume(float volume)
{
  return es8311_voice_volume_set(codec, 100 * constrain(volume, 0.0f, 1.0f), nullptr);
}

static esp_err_t codec_volume_percentage(int16_t volume)
{
  return es8311_voice_volume_set(codec, constrain(volume, 0, 100), nullptr);
}

static esp_err_t codec_deinit()
{
  if (codec) {
    es8311_delete(codec);
    codec = nullptr;
  }
  return ESP_OK;
}