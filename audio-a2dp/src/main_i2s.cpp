// #define USE_HELIX 

// #include <Arduino.h>

// #include "AudioTools.h"
// #include "AudioCodecs/CodecMP3Helix.h"

// URLStream url();
// //ICYStream icy();

// //AudioOutputWithCallback callback;
// I2SStream i2s; // final output of decoded stream
// SPDIFStream spdif;
// MetaDataPrint metadata; // final output of metadata

// EncodedAudioStream dec(&i2s, new MP3DecoderHelix()); // Decoding stream
// MultiOutput out(metadata, dec);

// StreamCopy copier(out, url); // copy url to decoder

// // callback for meta data
// void printMetaData(MetaDataType type, const char* str, int len){
//   Serial.print("==> ");
//   Serial.print(toStr(type));
//   Serial.print(": ");
//   Serial.println(str);
// }

// void setup(){
//   Serial.begin(115200);
//   AudioLogger::instance().begin(Serial, AudioLogger::Info);  

//   // mp3 radio
//   url.begin("http://stream.srg-ssr.ch/m/rsj/mp3_128","audio/mp3");//SWISS Jazz

//   // setup metadata
//   metadata.setCallback(printMetaData);
//   metadata.begin(url.httpRequest());

//   // setup SPDIF
//   // auto cfg = spdif.defaultConfig();
//   // cfg.pin_data = 27;
//   // cfg.sample_rate = 44100;
//   // cfg.channels = 2;
//   // cfg.bits_per_sample = 16;
//   // spdif.begin(cfg);

//   // setup i2s
//   auto config = i2s.defaultConfig(TX_MODE);
//   config.pin_ws = 25;
//   config.pin_bck = 26;
//   config.pin_data = 33;
//   config.channels = 2;
//   config.sample_rate = 384000;
//   config.bits_per_sample = 32;
//   i2s.begin(config);

//   // setup I2S based on sampling rate provided by decoder
//  // dec.setNotifyAudioChange(i2s);
//   dec.begin();
// }

// void loop(){
//   copier.copy();
// }