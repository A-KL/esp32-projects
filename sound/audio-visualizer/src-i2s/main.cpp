#include "AudioTools.h"

AudioInfo info(48000, 2, 32);

// I2SStream in;
// I2SStream out;
// StreamCopy copier(out, in); // copies sound into i2s

I2SStream i2s;
StreamCopy copier(i2s, i2s); // copies sound into i2s

// Arduino Setup
void setup(void) {  
  Serial.begin(115200);
  AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Info); 

  // // start I2S in
  // Serial.println("starting I2S...");
  // auto config_in = in.defaultConfig(RX_MODE);
  // config_in.copyFrom(info); 
  // config_in.i2s_format = I2S_STD_FORMAT;
  // config_in.is_master = false;
  // config_in.port_no = 0;
  // config_in.pin_bck = I2S_IN_BCK;
  // config_in.pin_ws = I2S_IN_WS;
  // config_in.pin_data = I2S_IN_SD;
  // // config_in.fixed_mclk = sample_rate * 256
  // // config_in.pin_mck = 2
  // //config_in.use_apll = true; // Maybe it helps?
  // in.begin(config_in);

  // // start I2S out
  // auto config_out = out.defaultConfig(TX_MODE);
  // config_out.copyFrom(info);
  // config_out.i2s_format = I2S_STD_FORMAT;
  // config_out.is_master = true;
  // config_out.port_no = 1;
  // config_out.pin_bck = I2S_BCK;
  // config_out.pin_ws = I2S_WS;
  // config_out.pin_data = I2S_SD;
  // out.begin(config_out);

  // start I2S in
  Serial.println("starting I2S...");
  auto config = i2s.defaultConfig(RXTX_MODE);
  config.copyFrom(info); 
  config.i2s_format = I2S_STD_FORMAT; // or try with I2S_LSB_FORMAT
  config.is_master = false;
  config.pin_ws = 4;     //PCM2706 WS;  PCM5102 WSEL
  config.pin_bck = 5;    //PCM2706 BCLK;  PCM5102 BCK
  config.pin_data = 7;   //PCM2706 DATA
  config.pin_data_rx = 6;   //PCM5102 DIN
  i2s.begin(config);

 Serial.println("I2S started...");
}

// Arduino loop - copy sound to out 
void loop() {
  copier.copy();
}