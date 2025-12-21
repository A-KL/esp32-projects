#pragma once

#include <Arduino.h>
extern "C" {
  #include "soc/pcnt_struct.h"
}
#include <driver/pcnt.h>

void IRAM_ATTR onTimer();

class _FreqCountESP
{
private:
  uint8_t mPin;
  uint8_t mTriggerPin;
  hw_timer_t *mTimer;
  pcnt_isr_handle_t mIsrHandle;

public:
  static volatile uint8_t sIsFrequencyReady;
  static volatile uint32_t sCount;
  static volatile uint32_t sFrequency;
  static volatile uint32_t sLastPcnt;

  static portMUX_TYPE sMux;

  inline uint8_t freqPin() const {
    return mPin;
  }

  _FreqCountESP();
  ~_FreqCountESP();

  // Frequency counter using internal interval timer.
  void begin(uint8_t pin, uint16_t timerMs, uint8_t hwTimerId = 0);
  void end();
  
  uint32_t read();
  uint8_t available();
};

extern _FreqCountESP FreqCountESP;