#include "AudioTools.h"
#include "gui.h"

AudioInfo info(44100, 2, 16);

I2SStream i2s;
CsvOutput<int16_t> csv(Serial);
StreamCopy copier(csv, i2s);

void setup(){
    Serial.begin(115200);
    AudioLogger::instance().begin(Serial, AudioLogger::Info);

    auto cfg = i2s.defaultConfig(RX_MODE);
    cfg.copyFrom(info);
    cfg.i2s_format = I2S_STD_FORMAT; // or try with I2S_LSB_FORMAT
    //cfg.is_master = true;
       
    // this module nees a master clock if the ESP32 is master
    //cfg.use_apll = false;  // try with yes
    //cfg.pin_mck = 3; 
    i2s.begin(cfg);

    // make sure that we have the correct channels set up
    csv.begin(info);
}

void loop(){
  copier.copy();
}