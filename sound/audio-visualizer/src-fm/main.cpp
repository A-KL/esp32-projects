#include "FreqCountESP.h"

int inputPin = 47; 
// int inputPin = I2S_WS;
int timerMs = 1000;

void setup()
{
  Serial.begin(115200);
  FreqCountESP.begin(inputPin, timerMs);
}

void loop()
{
  if (FreqCountESP.available())
  {
    uint32_t frequency = FreqCountESP.read();
    Serial.println(frequency);
  }
}