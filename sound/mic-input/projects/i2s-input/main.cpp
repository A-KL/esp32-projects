#include "AudioTools.h"
#include "gui.h"
#include <VuOutput.h>

AudioInfo info(44100, 2, 16);

I2SStream i2s;

CsvOutput<int16_t> csv(Serial);
VuMeter<int16_t> vu;
MultiOutput decoded_out;

StreamCopy copier(decoded_out, i2s);


const int output_format = 16;

void setup(){
    Serial.begin(115200);

    tft.init();
    tft.setRotation(TFT_ROTATE);
    tft.setSwapBytes(true);

    //tft.setFreeFont(&Orbitron_Medium_20);
    tft.loadFont(NotoSansBold15);

    tft.fillScreen(TFT_BLACK);

    gui_init();
    gui_set_input((int)1);

    AudioLogger::instance().begin(Serial, AudioLogger::Debug);

    auto cfg = i2s.defaultConfig(RX_MODE);
    cfg.copyFrom(info);
    //cfg.i2s_format = I2S_STD_FORMAT; // or try with I2S_LSB_FORMAT
    cfg.is_master = true;
    cfg.pin_ws = I2S_WS;
    cfg.pin_bck = I2S_SCK;
    cfg.pin_data = I2S_SD;
    cfg.bits_per_sample = output_format;
    // this module nees a master clock if the ESP32 is master
    //cfg.use_apll = false;  // try with yes
    //cfg.pin_mck = 3; 

    decoded_out.add(vu);
    //decoded_out.add(csv);

    i2s.begin(cfg);

    // make sure that we have the correct channels set up
    csv.begin(info);
    vu.begin(info);

    gui_run(0);
}

void loop(){
  copier.copy();

  left_pb.value = vu.value_left();
  right_pb.value = vu.value_right();

  //log_e("Left: %f Right: %f", vu.value_left(), vu.value_right());
}