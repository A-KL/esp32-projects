
#define USE_A2DP

#include "BluetoothA2DPSink.h"
//#include "AudioConfigLocal.h"
#include "AudioTools.h"

BluetoothA2DPSink a2dp_sink;
SPDIFStream spdif;
I2SStream i2s; // final output of decoded stream

// Write data to SPDIF in callback
void read_data_stream(const uint8_t *data, uint32_t length) {
    i2s.write(data, length);
    Serial.println(length);
}

void setup() {
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Info);
  
  // register callback
  a2dp_sink.set_stream_reader(read_data_stream, false);

  // Start Bluetooth Audio Receiver
  a2dp_sink.set_auto_reconnect(false);
  a2dp_sink.start("a2dp-spdif");

  // setup output
//   auto cfg = spdif.defaultConfig();
//   cfg.pin_data = 27;
//   cfg.sample_rate = a2dp_sink.sample_rate();
//   cfg.channels = 2;
//   cfg.bits_per_sample = 16;
//   spdif.begin(cfg);

    // setup i2s
  auto config = i2s.defaultConfig();
  config.pin_ws = 25;
  config.pin_bck = 26;
  config.pin_data = 33;
  config.channels = 2;
  config.sample_rate = 44100;
  config.bits_per_sample = 16;
  //config.fixed_mclk = 12288000; // 2 * 2 * a2dp_sink.sample_rate() * 2 * 16;
  config.use_apll = true;
  i2s.begin(config);

	REG_WRITE(PIN_CTRL, 0xFF0); 
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0_CLK_OUT1);

}

void loop() {
  delay(100);
}

// A2DPStream a2dp = A2DPStream::instance(); // access A2DP as stream
// //I2SStream i2sStream;                    // Access I2S as stream
// SPDIFStream spdif;


// VolumeStream volume(a2dp);
// StreamCopy copier(volume, spdif); // copy i2sStream to a2dpStream to spdif

// //ConverterFillLeftAndRight<int16_t> filler(LeftIsEmpty); // fill both channels

// void setupBluetooth()
// {
//     Serial.begin(115200);

//     // set intial volume
//     volume.setVolume(0.3);

//     // start SPDIF
//     Serial.println("starting SPDIF...");

//     auto config = spdif.defaultConfig();
//     config.sample_rate = 44100; 
//     config.channels = 2;
//     config.pin_data = 27;
    
//     // start bluetooth
//     Serial.println("starting A2DP...");

//     auto configA2DP = a2dp.defaultConfig(RX_MODE);
//     configA2DP.name = "LEXON MINO L";
//     a2dp.begin(configA2DP);

//     // start i2s input with default configuration
//     Serial.println("starting I2S...");

//     a2dp.setNotifyAudioChange(spdif);
//     spdif.begin(config);
// }

// void loopBluetooth()
// {
//    // copier.copy(filler);
//    copier.copy();
// }