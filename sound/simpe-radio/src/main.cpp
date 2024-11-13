// #define NO_TRACE
// #define LOG_NO_MSG
// #define COPY_LOG_OFF
// #define LOG_LEVEL AudioLogger::Error

#include "Creds.h"
#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"

URLStream url(LOCAL_SSID, LOCAL_PASSWORD);
I2SStream i2s; // final output of decoded stream
NumberFormatConverterStream nfc(i2s);
EncodedAudioStream dec(&nfc, new MP3DecoderHelix()); // Decoding stream
StreamCopy copier(dec, url); // copy url to decoder

void setup(){
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Info);
  
  //nfc.addNotifyAudioChange(i2s);
  // convert 16 bits to 32, you could also change the gain
  nfc.begin(16, 32);
  // setup i2s
  auto config = i2s.defaultConfig(TX_MODE);
  // you could define e.g your pins and change other settings
  config.pin_ws = I2S_WS;
  config.pin_bck = I2S_BCK;
  config.pin_data = I2S_SD;
  //config.pin_mck = I2S_MCLK;
  config.bits_per_sample = 32;
  config.is_master = true;
  //config.i2s_format = I2S_LSB_FORMAT;

  i2s.begin(config);

  // setup I2S based on sampling rate provided by decoder
  dec.begin();

// mp3 radio
  url.begin("http://stream.srg-ssr.ch/m/rsj/mp3_128","audio/mp3");
}

void loop(){
  copier.copy();
}


/**
 * @file streams-generator-i2s.ino
 * @author Phil Schatzmann
 * @brief see https://github.com/pschatzmann/arduino-audio-tools/blob/main/examples/examples-stream/streams-generator-i2s/README.md 
 * @copyright GPLv3
 */
 
// #include "AudioTools.h"

// AudioInfo info(48000, 2, 16); //48000

// SineWaveGenerator<int16_t> sineWave(32000);          // subclass of SoundGenerator with max amplitude of 32000
// GeneratedSoundStream<int16_t> sound(sineWave);       // Stream generated from sine wave
// I2SStream out; 
// NumberFormatConverterStream nfc(out);
// StreamCopy copier(nfc, sound);                       // copies sound into i2s

// // Arduino Setup
// void setup(void) {  
//   Serial.begin(115200);
//   //AudioLogger::instance().begin(Serial, AudioLogger::Error);
//   AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Error);

//   //nfc.addNotifyAudioChange(out); 
//   nfc.begin(16, 32);

//   // start I2S
//   Serial.println("starting I2S...");
//   auto config = out.defaultConfig(TX_MODE);
//   config.copyFrom(info); 

//   config.pin_ws = I2S_WS;
//   config.pin_bck = I2S_BCK;
//   config.pin_data = I2S_SD;
//   config.bits_per_sample = 32;

//   out.begin(config);

//   // Setup sine wave
//   sineWave.begin(info, N_B4);
//   Serial.println("started...");
// }

// // Arduino loop - copy sound to out 
// void loop() {
//   copier.copy();
// }