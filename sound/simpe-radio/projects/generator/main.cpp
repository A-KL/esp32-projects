
/**
 * @file streams-generator-i2s.ino
 * @author Phil Schatzmann
 * @brief see https://github.com/pschatzmann/arduino-audio-tools/blob/main/examples/examples-stream/streams-generator-i2s/README.md 
 * @copyright GPLv3
 */
 
#include "AudioTools.h"

AudioInfo info(I2S_SAMPLE_RATE, 2, I2S_BPS);

SineWaveGenerator<int16_t> sineWave(32000);          // subclass of SoundGenerator with max amplitude of 32000
GeneratedSoundStream<int16_t> sound(sineWave);       // Stream generated from sine wave
I2SStream out; 
NumberFormatConverterStream nfc(out);
StreamCopy copier(nfc, sound);                       // copies sound into i2s

// Arduino Setup
void setup(void) {  
  Serial.begin(115200);
  //AudioLogger::instance().begin(Serial, AudioLogger::Error);
  AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Error);

  //nfc.addNotifyAudioChange(out); 
  nfc.begin(16, I2S_BPS);

  // start I2S
  Serial.println("starting I2S...");
  auto config = out.defaultConfig(TX_MODE);
  config.copyFrom(info); 

  config.pin_ws = I2S_WS;
  config.pin_bck = I2S_BCK;
  config.pin_data = I2S_SD;
  config.bits_per_sample = I2S_BPS;

  out.begin(config);

  // Setup sine wave
  sineWave.begin(info, N_B4);
  Serial.println("started...");
}

void loop() {
  copier.copy();
}