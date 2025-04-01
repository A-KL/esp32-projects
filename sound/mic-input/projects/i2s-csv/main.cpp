#include "AudioTools.h"

AudioInfo info(44100, 2, 16);
I2SStream i2s;
//MultiOutput decoded_out;
CsvOutput<int16_t> csv(Serial);
StreamCopy copier(csv, i2s);

void setup(){
    Serial.begin(115200);
    //AudioLogger::instance().begin(Serial, AudioLogger::Warning);
    //AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Warning);

    auto cfg = i2s.defaultConfig(RX_MODE);
    cfg.copyFrom(info);
    cfg.is_master = true;
    cfg.pin_ws = I2S_WS;
    cfg.pin_bck = I2S_BCK;
    cfg.pin_data = I2S_SD;
    cfg.i2s_format = I2S_STD_FORMAT;
    //cfg.use_apll = false;  
    //decoded_out.add(csv);
    i2s.begin(cfg);
    csv.begin(info);
}

void loop(){
  copier.copy();
}