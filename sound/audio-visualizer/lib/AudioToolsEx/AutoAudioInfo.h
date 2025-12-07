#pragma once

#include <math.h>

#include "AudioTools/CoreAudio/AudioTypes.h"

#include "FreqCountESP.h"

namespace audio_tools {

const uint32_t SampleRatesDefs[] = { 8000L, 16000L, 44100L, 48000L, 96000L, 192000L };

class AutoAudioInfo {
  public:
    AutoAudioInfo() = default;

    AutoAudioInfo(AudioInfo info) : _info(info)
    {}

    void begin(uint8_t pin_ws, uint8_t pin_bck, int wait_ms = 500) {
      _pin_ws = pin_ws;
      _pin_bck = pin_bck;

      FreqCountESP.begin(_pin_ws, wait_ms);
    }

    void end() {
      FreqCountESP.end();
    }

    void loop() {
      if (FreqCountESP.available()) {
        auto frequency = FreqCountESP.read();

        if (frequency < SampleRatesDefs[0]) {
          return;
        }

        for (const auto &i : SampleRatesDefs) {
        
          if (std::abs(static_cast<long>(i - frequency)) < 10) {
            LOGW("AutoAudioInfo. Detected: %u Mapped: %u", frequency, i);
            break;
          }
        }
      }
    }

  private:
    AudioInfo _info;
    uint8_t _pin_ws;
    uint8_t _pin_bck;

};

}