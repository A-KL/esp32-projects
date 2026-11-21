#include "AudioTools.h"
// #include <Adafruit_NeoPixel.h> 
// // // #include "I2SAutoStream.h"
// // #include "AutoAudioInfo.h"

// // #define RGB_LED_PIN 21

// // AudioInfo info(48000, 2, 16);
// // AutoAudioInfo detector(info);

// // // // I2SStream in;
// // // // I2SStream out;
// // // // StreamCopy copier(out, in); // copies sound into i2s

// // I2SStream i2s;
// // StreamCopy copier(i2s, i2s); // copies sound into i2s

// // Adafruit_NeoPixel rgb(1, RGB_LED_PIN);

// // Arduino Setup
// // void setup(void) {  
// //   Serial.begin(115200);
// //   AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Info); 

// // //   // // start I2S in
// // //   // Serial.println("starting I2S...");
// // //   // auto config_in = in.defaultConfig(RX_MODE);
// // //   // config_in.copyFrom(info); 
// // //   // config_in.i2s_format = I2S_STD_FORMAT;
// // //   // config_in.is_master = false;
// // //   // config_in.port_no = 0;
// // //   // config_in.pin_bck = I2S_IN_BCK;
// // //   // config_in.pin_ws = I2S_IN_WS;
// // //   // config_in.pin_data = I2S_IN_SD;
// // //   // // config_in.fixed_mclk = sample_rate * 256
// // //   // // config_in.pin_mck = 2
// // //   // //config_in.use_apll = true; // Maybe it helps?
// // //   // in.begin(config_in);

// // //   // // start I2S out
// // //   // auto config_out = out.defaultConfig(TX_MODE);
// // //   // config_out.copyFrom(info);
// // //   // config_out.i2s_format = I2S_STD_FORMAT;
// // //   // config_out.is_master = true;
// // //   // config_out.port_no = 1;
// // //   // config_out.pin_bck = I2S_BCK;
// // //   // config_out.pin_ws = I2S_WS;
// // //   // config_out.pin_data = I2S_SD;
// // //   // out.begin(config_out);

// //   detector.begin(47, 48);

// //   // start I2S in
// //   Serial.println("starting I2S...");
// //   auto config = i2s.defaultConfig(RXTX_MODE);
// //   config.copyFrom(info); 
// //   config.i2s_format = I2S_STD_FORMAT; // or try with I2S_LSB_FORMAT
// //   config.is_master = false;
// //   config.pin_ws = 47;   // 4;     //PCM2706 WS;    PCM5102 WSEL
// //   config.pin_bck = 48;  // 5;    //PCM2706 BCLK;  PCM5102 BCK
// //   config.pin_data = 36; // 6;   //PCM2706 DATA
// //   config.pin_data_rx = 35;  // 3;               // PCM5102 DIN
// //   i2s.begin(config);

// //   rgb.begin();
// //   rgb.clear();
// //   rgb.setPixelColor(0, rgb.Color(0, 100, 50));
// //   rgb.show();  
// //   Serial.println("I2S started...");
// // }

// // void loop() {
// //   copier.copy();
// //   detector.loop();
// // }

/* RADIO */

#include "AudioTools/AudioCodecs/CodecMP3Helix.h"
#include "AudioTools/Communication/AudioHttp.h"

URLStream url(WIFI_SSID, WIFI_PASSWORD);
I2SStream i2s;
NumberFormatConverterStream nfc(i2s);
EncodedAudioStream dec(&nfc, new MP3DecoderHelix());
StreamCopy copier(dec, url);

AudioInfo from(44100,2, 16);
AudioInfo to(44100,2, 32);

void setup(){
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Info);
  
  nfc.addNotifyAudioChange(i2s);
  nfc.begin(from, to);

  // setup i2s
  auto config = i2s.defaultConfig(TX_MODE);
  config.copyFrom(to); 
  config.pin_ws = 2; // 7; // 4;
  config.pin_bck = 3;//8; //5;
  config.pin_data = 6;//9; //6;
  config.is_master = true;
  i2s.begin(config);

  // setup I2S based on sampling rate provided by decoder
  dec.begin(from);

// mp3 radio
  url.begin("http://stream.srg-ssr.ch/m/rsj/mp3_128","audio/mp3");
}

/* GENERATOR */

// AudioInfo from(44100, 2, 16);
// AudioInfo to(44100, 2, 32);

// SineWaveGenerator<int16_t> sineWave;  // subclass of SoundGenerator with max amplitude of 32000
// GeneratedSoundStream<int16_t> generator(sineWave);

// I2SStream out;
// FormatConverterStream converter(out);
// // NumberFormatConverterStream converter(out);

// StreamCopy copier(converter, generator);
// bool i2s_started = false;

// // Arduino Setup
// void setup(void) {
//   // Open Serial 
//   Serial.begin(115200);
//   AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Warning);
//   delay(3000);

//   // Setup Input
//   sineWave.begin(from, N_B4);
//   generator.begin(from);

//   // Setup Converter
//   converter.addNotifyAudioChange(out);
//   // converter.begin(16, 32);
//   converter.begin(from, to);

//   // Start Output
//   Serial.println("starting I2S...");
//   auto config = out.defaultConfig(TX_MODE);
//   config.copyFrom(to);
//   config.pin_ws = 7; // 4;
//   config.pin_bck = 8; //5;
//   config.pin_data = 9; //6;
//   // config.pin_mck = 10;
//   config.bits_per_sample = 32;
//   config.is_master = true;
//   Serial.printf("I2S config: %d Hz, %d channels, %d bits; BCK=%d WS=%d DATA=%d\n",
//                 config.sample_rate, config.channels, config.bits_per_sample,
//                 config.pin_bck, config.pin_ws, config.pin_data);
//   i2s_started = out.begin(config);
//   Serial.println(i2s_started ? "I2S started" : "ERROR: I2S start failed");
// }

void loop(){
  copier.copy();
}
