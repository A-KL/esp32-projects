#pragma once

#include <math.h>

#include "AudioTools/CoreAudio/AudioTypes.h"

#include "FreqCountESP.h"

namespace audio_tools {

const uint32_t SampleRates[] = { 8000L, 16000L, 44100L, 48000L, 96000L, 192000L };
const uint8_t SampleSizes[] = { 8, 16, 24, 32 };

class AutoAudioInfo {
  public:
    AutoAudioInfo() = default;

    AutoAudioInfo(AudioInfo info) : _info(info), _fraction(1)
    {}

    bool begin(uint8_t pin_ws, uint8_t pin_bck, int wait_ms = 200) {
      _pin_ws = pin_ws;
      _pin_bck = pin_bck;
      _fraction = 1000 / wait_ms;

      auto result = true;

      result &= FreqCountESP.addPin(_pin_ws);
      result &= FreqCountESP.addPin(_pin_bck);
      result &= FreqCountESP.begin(wait_ms);

      return result;
    }

    void end() {
      FreqCountESP.end();
    }

    bool detect(AudioInfo& info) 
    {
      auto sample_rate_updated = false;

      if (FreqCountESP.available(0)) // LCK
      {
        auto frequency = FreqCountESP.read(0) * _fraction;

        for (const auto &rate : SampleRates) 
        {
          if (std::abs(static_cast<long>(rate - frequency)) >= _threshold_ws) {
            continue;
          }

          if (_info.sample_rate != rate) {
            _info.sample_rate = rate;
            sample_rate_updated = true;
          }
          break;
        }
        
        LOGI("AutoAudioInfo. Detected: %u Mapped: %u Updated: %d", frequency, _info.sample_rate, sample_rate_updated);
      }

      auto sample_size_updated = false;

      if (FreqCountESP.available(1)) // SCK
      {
        auto frequency = FreqCountESP.read(1) * _fraction;
        auto sample_size = frequency / _info.sample_rate / 2;

        for (const auto &sample : SampleSizes) 
        {
          if (std::abs(sample - sample_size) >= _threshold_bck) {
            continue;
          }

          if (_info.bits_per_sample != sample) {
            _info.bits_per_sample = sample;
            sample_size_updated = true;
          }
          break;
        }
      }

      return sample_rate_updated || sample_size_updated;
    }

  private:
    AudioInfo _info;
    uint8_t _pin_ws;
    uint8_t _pin_bck;
    float _fraction;
    
    const uint8_t _threshold_ws = 10;
    const uint8_t _threshold_bck = 2;
};

}