#include <cstdarg>
#include <iostream>

#include <FreqCountESP.h>

#define PCNT_HIGH_LIMIT INT16_MAX //32767 largest +ve value for int16_t.
#define PCNT_LOW_LIMIT  0
//#define PCNT_UNIT PCNT_UNIT_0
#define PCNT_CHANNEL PCNT_CHANNEL_0

portMUX_TYPE pcntMux = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE _FreqCountESP::sMux = portMUX_INITIALIZER_UNLOCKED;
volatile pcnt_context_t _FreqCountESP::units[PCNT_UNIT_MAX];

static void IRAM_ATTR onHLim(void *context)
{
  auto counter = (_FreqCountESP*)context;

  // 16 bit pulse counter hit high limit; increment the 32 bit backup.
  portENTER_CRITICAL_ISR(&pcntMux);

  auto intr_status = PCNT.int_st.val;

  for (auto i{0}; i < PCNT_UNIT_MAX; i++) {
    if (intr_status & (BIT(i))) {
      counter->units[i].count += PCNT_HIGH_LIMIT;
      PCNT.int_clr.val = BIT(i);
    }
  }
  // counter->units[0].count += PCNT_HIGH_LIMIT;
  // PCNT.int_clr.val = BIT(PCNT_UNIT);  // Clear the interrupt.
  portEXIT_CRITICAL_ISR(&pcntMux);
}

void IRAM_ATTR onTimer()
{
  portENTER_CRITICAL_ISR(&_FreqCountESP::sMux);
  for (auto unit_index = 0; unit_index < PCNT_UNIT_MAX; unit_index++)
  {
    int16_t pulseCount;
    uint32_t pcntTotal = _FreqCountESP::units[unit_index].count;
    pcnt_get_counter_value((pcnt_unit_t)unit_index, &pulseCount);
    if (pulseCount < 1000) {
      // Maybe counter just rolled over? Re-read 32 bit basis.
      pcntTotal = _FreqCountESP::units[unit_index].count;
    }
    pcntTotal += pulseCount;
    _FreqCountESP::units[unit_index].frequency = (uint32_t)(pcntTotal - _FreqCountESP::units[unit_index].last);
    _FreqCountESP::units[unit_index].last = pcntTotal;
    _FreqCountESP::units[unit_index].isFrequencyReady = true;
  }

  portEXIT_CRITICAL_ISR(&_FreqCountESP::sMux);
}

static void setupPcnt(pcnt_isr_handle_t &handle, uint8_t pin, pcnt_unit_t pcntUnit, volatile void* context) { 
  pcnt_config_t pcntConfig = {
    .pulse_gpio_num = pin,
    .ctrl_gpio_num = -1,
    .pos_mode = PCNT_CHANNEL_EDGE_ACTION_INCREASE,
    .neg_mode = PCNT_CHANNEL_EDGE_ACTION_HOLD,
    .counter_h_lim = PCNT_HIGH_LIMIT,
    .counter_l_lim = PCNT_LOW_LIMIT,
    .unit = pcntUnit,
    .channel = PCNT_CHANNEL,
  };
  pcnt_unit_config(&pcntConfig);
  pcnt_counter_pause(pcntUnit);
  pcnt_counter_clear(pcntUnit);
  pcnt_event_enable(pcntUnit, PCNT_EVT_H_LIM);  // Interrupt on high limit.
  
  pcnt_isr_register(onHLim, (void *)context, 0, &handle);
  pcnt_intr_enable(pcntUnit);
  pcnt_counter_resume(pcntUnit);
}

static void teardownPcnt(pcnt_isr_handle_t handle, pcnt_unit_t unit)
{  
  pcnt_counter_pause(unit);
  pcnt_intr_disable(unit);
  pcnt_isr_unregister(handle);
}

bool _FreqCountESP::begin(uint16_t timerMs, uint8_t hwTimerId)
{
  if (mPinsCount == 0) {
    // LOG_E
    return false;
  }

  // Configure counting on frequency input pin.
  for (auto unit_index = 0; unit_index < PCNT_UNIT_MAX; unit_index++)
  {
    // Static
    _FreqCountESP::units[unit_index].isFrequencyReady = false;
    _FreqCountESP::units[unit_index].count = 0;
    _FreqCountESP::units[unit_index].frequency = 0;
    _FreqCountESP::units[unit_index].last = 0;
  }

  // Count frequency using internal timer.
  mTimer = timerBegin(hwTimerId, 80, true);
  timerAttachInterrupt(mTimer, &onTimer, true);
  timerAlarmWrite(mTimer, timerMs * 1000, true);

  for (auto pin_index = 0; pin_index < mPinsCount; pin_index++)
  {
    setupPcnt(mIsrHandles[pin_index], mPins[pin_index], (pcnt_unit_t)pin_index, this);
  }
  
  timerAlarmEnable(mTimer);

  return true;
}

// void _FreqCountESP::begin(uint16_t timerMs, uint8_t hwTimerId, ...)
// {
//   va_list args;
//   va_start(args, hwTimerId);



//   va_end(args);
// }

uint32_t _FreqCountESP::read(uint8_t index)
{
  units[index].isFrequencyReady = false;
  return units[index].frequency;
}

uint8_t _FreqCountESP::available(uint8_t index)
{
  return units[index].isFrequencyReady;
}

void _FreqCountESP::end()
{
  for (auto i = 0; i < mPinsCount; i++)
  {
    teardownPcnt(mIsrHandles[i], (pcnt_unit_t)i);
  }
  
  timerAlarmDisable(mTimer);
  timerDetachInterrupt(mTimer);
  timerEnd(mTimer);
}

_FreqCountESP::_FreqCountESP()
{
  mTimer = NULL;

  for (auto unit_index = 0; unit_index < PCNT_UNIT_MAX; unit_index++)
    mPins[unit_index] = -1;
}

_FreqCountESP::~_FreqCountESP()
{
  end();
}

_FreqCountESP FreqCountESP;