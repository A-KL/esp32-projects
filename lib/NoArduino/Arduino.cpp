#include "Arduino.h"

#include <chrono>
#include <iostream>
#include <thread>

unsigned long millis()
{
  const auto now = std::chrono::steady_clock::now();
  const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
  return duration.count();
}

void delay(uint32_t ms)
{
  std::this_thread::sleep_for(std::chrono::microseconds(ms));
}

void delayMicroseconds(uint32_t us)
{
  std::this_thread::sleep_for(std::chrono::nanoseconds(us));
}