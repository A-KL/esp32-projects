//#include "FreqCountESP.h"

#include "AutoAudioInfo.h"

using namespace audio_tools;

AudioInfo info(44100, 2, 16);
AutoAudioInfo auto_info(info);

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
  AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Info);
  delay(2000);

  pinMode(LCK, INPUT);
  pinMode(BCK, INPUT);

  auto_info.begin(LCK, BCK);

  // FreqCountESP.addPin(LCK);
  // FreqCountESP.addPin(BCK);

  // FreqCountESP.begin(timerMs);

  Serial.println("Start");
}

AudioInfo info_out(44100, 2, 16);

void loop()
{
  if (auto_info.detect(info_out))
  {
    Serial.printf("AudioInfo: %d, %d, %u\r\n", info_out.bits_per_sample, info_out.channels, info_out.sample_rate);
  }

  // if (FreqCountESP.available())
  // {
  //   last_freq = FreqCountESP.read();
  //  // Serial.print("Pin: ");
  //  // Serial.print(FreqCountESP.pin());
  //  // Serial.print(" Freq: ");
  //  // Serial.println(last_freq * mul);
  // }

  // if (FreqCountESP.available(1))
  // {
  //   uint32_t frequency = FreqCountESP.read(1);
  //   // Serial.print("Pin: ");
  //   // Serial.print(FreqCountESP.pin(1));
  //   // Serial.print(" Freq: ");
  //   // Serial.println(frequency * mul);

  //   Serial.printf("Freq: %u Hz. Sample size: %u\r\n", last_freq * mul, (frequency / last_freq / 2));
  // }
}