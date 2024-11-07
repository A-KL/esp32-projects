// #define NO_TRACE
// #define LOG_NO_MSG
// #define COPY_LOG_OFF
// #define LOG_LEVEL AudioLogger::Error

#include "AudioTools.h"

//AudioInfo info(44100, 2, 32);
AudioInfo info(16000, 2, 16);
I2SStream i2sStream; // Access I2S as stream
CsvOutput<int16_t> csvStream(Serial);
StreamCopy copier(csvStream, i2sStream); // copy i2sStream to csvStream

void setup(){
    Serial.begin(115200);
    AudioLogger::instance().begin(Serial, AudioLogger::Warning);

    auto cfg = i2sStream.defaultConfig(RX_MODE);
    cfg.copyFrom(info);

    cfg.pin_bck = I2S_BCK;
    cfg.pin_ws = I2S_WS;
    cfg.pin_data = I2S_SD;
    cfg.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT;

    // cfg.is_master = true; 
    // cfg.i2s_format = I2S_STD_FORMAT; // or try with I2S_LSB_FORMAT

    //   // this module nees a master clock if the ESP32 is master
    // cfg.use_apll = false;  // try with yes
    // cfg.pin_mck = 3;

    i2sStream.begin(cfg);

    // make sure that we have the correct channels set up
    csvStream.begin(info);
}

void loop(){
  copier.copy();
}
