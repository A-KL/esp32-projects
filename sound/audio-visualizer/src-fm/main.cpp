//#include "FreqCountESP.h"
// #include "freq_counter_t.h"

#include "AutoAudioInfo.h"
// using namespace audio_tools;
// AudioInfo info(44100, 2, 16);
// AutoAudioInfo auto_info(info);

#define LCK 6
#define BCK 5

static int inputPin = 6;
static int timerMs = 200;
static int mul = 1000 / timerMs;

static uint32_t last_freq = 1;
static uint32_t last_bits = 1;

void setup()
{
  Serial.begin(115200);

  pinMode(LCK, INPUT);
  pinMode(BCK, INPUT);

  FreqCountESP.addPin(LCK);
  FreqCountESP.addPin(BCK);

  FreqCountESP.begin(timerMs);

  Serial.println("Start");
}

void loop()
{
  if (FreqCountESP.available())
  {
    last_freq = FreqCountESP.read();
   // Serial.print("Pin: ");
   // Serial.print(FreqCountESP.pin());
   // Serial.print(" Freq: ");
   // Serial.println(last_freq * mul);
  }

  if (FreqCountESP.available(1))
  {
    uint32_t frequency = FreqCountESP.read(1);
    // Serial.print("Pin: ");
    // Serial.print(FreqCountESP.pin(1));
    // Serial.print(" Freq: ");
    // Serial.println(frequency * mul);

    Serial.printf("Freq: %u Hz. Sample size: %u\r\n", last_freq * mul, (frequency / last_freq / 2));
  }
}