#include "AudioTools.h"
#include "AudioTools/AudioLibs/VBANStream.h"

#include "Creds.h"
#include "gui.h"
#include <VuOutput.h>


AudioInfo info(I2S_SAMPLE_RATE, 2, I2S_BPS);

I2SStream i2s;
CsvOutput<int16_t> csv(Serial);
VuMeter<int16_t> vu(0.05);
VBANStream vban;
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
    gui_set_input((int)1);

    AudioLogger::instance().begin(Serial, AudioLogger::Info);

    auto cfg = i2s.defaultConfig(RX_MODE);
    cfg.copyFrom(info);
    //cfg.i2s_format = I2S_STD_FORMAT; // or try with I2S_LSB_FORMAT
    cfg.is_master = true;
    cfg.pin_ws = I2S_WS;
    cfg.pin_bck = I2S_SCK;
    cfg.pin_data = I2S_SD;
    cfg.bits_per_sample = I2S_BPS;
    // this module nees a master clock if the ESP32 is master
    //cfg.use_apll = false;  // try with yes
    //cfg.pin_mck = 3; 

    // Network
    auto vban_cfg = vban.defaultConfig(TX_MODE);
    vban_cfg.copyFrom(info);
    vban_cfg.ssid = LOCAL_SSID;
    vban_cfg.password = LOCAL_PASSWORD;
    vban_cfg.stream_name = "av-receiver-stream";
    vban_cfg.target_ip = IPAddress{0,0,0,0}; 
    vban_cfg.throttle_active = true;

    // Decoders
    decoded_out.add(vu);
    decoded_out.add(vban);
   // decoded_out.add(csv);

    i2s.begin(cfg);
   // csv.begin(info);
    vu.begin(info);

    if (!vban.begin(vban_cfg)) {
      stop();
    }

    gui_run(0);
}

void loop(){
  copier.copy();

  left_pb.value = vu.value_left();
  right_pb.value = vu.value_right();
}