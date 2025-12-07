#pragma once

#include <math.h>
#include "AudioToolsConfig.h"

#if defined(USE_I2S)

#include "AudioTools/CoreAudio/AudioI2S/I2SStream.h"
// #include "AudioTools/CoreAudio/AudioI2S/I2SESP32.h"
// #include "AudioTools/CoreAudio/AudioI2S/I2SESP32V1.h"
// #include "AudioTools/CoreAudio/AudioI2S/I2SESP8266.h"
// #include "AudioTools/CoreAudio/AudioI2S/I2SNanoSenseBLE.h"
// #include "AudioTools/CoreAudio/AudioI2S/I2SRP2040-MBED.h"
// #include "AudioTools/CoreAudio/AudioI2S/I2SRP2040.h"
// #include "AudioTools/CoreAudio/AudioI2S/I2SSAMD.h"
// #include "AudioTools/CoreAudio/AudioI2S/I2SSTM32.h"
// #include "AudioTools/CoreAudio/AudioStreams.h"
// #include "AudioTools/CoreAudio/AudioTypes.h"

#include "FreqCountESP.h"

#if defined(IS_I2S_IMPLEMENTED)

namespace audio_tools {

const uint32_t SampleRatesDefs[] = { 8000L, 16000L, 44100L, 48000L, 96000L, 192000L };

class I2SAutoStream : public I2SStream
{
public:
  I2SAutoStream() = default;
  ~I2SAutoStream() { end(); }

  void end() {
    I2SStream::end();
  };

// private:

  void detect() {

    auto cfg = i2s.config();

    FreqCountESP.begin(cfg.pin_ws, 500);

    while (FreqCountESP.available())
    {
      delay(100);

      auto frequency = FreqCountESP.read();

      if (frequency < SampleRatesDefs[0]) {
        continue;
      }

      for (const auto &i : SampleRatesDefs) {
          if (std::abs(static_cast<long>(i - frequency)) < 10) {

            LOGW("I2SAutoStream. Detected: %u Mapped: %u", frequency, i);
            break;
          }
      } 
    }
  }
};

}

#endif

#endif