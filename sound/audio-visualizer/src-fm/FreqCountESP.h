#pragma once

#include <Arduino.h>
extern "C" {
  #include "soc/pcnt_struct.h"
}
#include <driver/pcnt.h>

struct pcnt_context_t {
    volatile uint8_t isFrequencyReady = false;
    volatile uint32_t count = 0;
    volatile uint32_t frequency = 0;
    volatile uint32_t last = 0;
};

void IRAM_ATTR onTimer();

class _FreqCountESP
{
private:
  int8_t mPins[PCNT_UNIT_MAX];
  uint8_t mPinsCount = 0;
  hw_timer_t *mTimer;
  pcnt_isr_handle_t mIsrHandles[PCNT_UNIT_MAX];

  void clear();

public:
  static volatile pcnt_context_t units[PCNT_UNIT_MAX];
  static portMUX_TYPE sMux;

  inline uint8_t pin(uint8_t index = 0) const {
    return mPins[index];
  }

  inline uint8_t pinsCount() const {
    return mPinsCount;
  }

  bool addPin(const uint8_t pin)
  {
    if (PCNT_UNIT_MAX > mPinsCount + 1)
    {
      mPins[mPinsCount] = pin;
      mPinsCount++;
      return true;
    }
    return false;
  }

  _FreqCountESP();
  ~_FreqCountESP();

  // Frequency counter using internal interval timer.
  bool begin(uint16_t timerMs, uint8_t hwTimerId = 0);
  void end();
  
  uint32_t read(uint8_t index = 0);
  uint8_t available(uint8_t index = 0);

};

extern _FreqCountESP FreqCountESP;