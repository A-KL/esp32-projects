#include "AudioTools.h"
#include "gui.h"
#include <VuOutput.h>

AudioInfo info(I2S_SAMPLE_RATE, I2S_CHANNELS, I2S_BPS);

I2SStream i2s;
//CsvOutput<int16_t> csv(Serial);
VuMeter<int16_t> vu(AUDIO_VU_RATIO);
MultiOutput decoded_out;
StreamCopy copier(decoded_out, i2s);

void setup(){
    Serial.begin(115200);

    tft.init();
    tft.setRotation(TFT_ROTATE);
    tft.setSwapBytes(true);

    //tft.setFreeFont(&Orbitron_Medium_20);
    tft.loadFont(NotoSansBold15);

    tft.fillScreen(TFT_BLACK);

    gui_init();
    //gui_set_input((int)1);

    AudioLogger::instance().begin(Serial, AudioLogger::Warning);

    auto cfg = i2s.defaultConfig(RX_MODE);
    cfg.copyFrom(info);
    cfg.is_master = I2S_MASTER;
    cfg.pin_ws = I2S_WS;
    cfg.pin_bck = I2S_BCK;
    cfg.pin_data = I2S_SD;
    //cfg.use_apll = false;  // try with yes
    //cfg.pin_mck = 3; 

    decoded_out.add(vu);
    //decoded_out.add(csv);

    i2s.begin(cfg);

    // make sure that we have the correct channels set up
    //csv.begin(info);
    vu.begin(info);

    gui_begin();
}

void loop(){
  copier.copy();

  left_pb.value = vu.value_left();
  right_pb.value = vu.value_right();

  gui_update();
}