#include <Arduino.h>
#include <Wire.h>
#include <ArduinoLog.h>

#include <AS5600.h>

AS5600L encoder(AS5600_DEFAULT_ADDRESS);   //  use default Wire

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.begin(115200);

  // Log.begin(LOG_LEVEL_VERBOSE, &Serial);
  // Log.infoln(AS5600_LIB_VERSION);

  Serial.println(AS5600_LIB_VERSION);

  Wire.begin();

  if (!encoder.begin()) {
    Serial.println("AS5600 not detected. Check connections.");
    while (1);
  }

  Serial.println("AS5600 initialized.");

  // encoder.begin(4);  //  set direction pin.
  // encoder.setDirection(AS5600_CLOCK_WISE);
  // encoder.setOutputMode(AS5600_OUTMODE_ANALOG_100);
}

void loop()
{
  Serial.println(encoder.readAngle());

}