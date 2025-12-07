#include "AudioTools.h"

#include "gui.h"
#include <VuOutput.h>

AudioInfo info(I2S_SAMPLE_RATE, I2S_CHANNELS, I2S_BPS);

I2SStream i2s;
VuMeter<int16_t> vu(AUDIO_VU_RATIO);
MultiOutput decoded_out;
StreamCopy copier(decoded_out, i2s);

void setup(){
    Serial.begin(115200);
    AudioLogger::instance().begin(Serial, AudioLogger::Warning);

    gui_init();

    auto cfg = i2s.defaultConfig(RX_MODE);
    cfg.copyFrom(info);
    cfg.is_master = I2S_MASTER;
    cfg.pin_ws = I2S_WS;
    cfg.pin_bck = I2S_BCK;
    cfg.pin_data = I2S_SD;

    // Network
    // auto vban_cfg = vban.defaultConfig(TX_MODE);
    // vban_cfg.copyFrom(info);
    // vban_cfg.ssid = LOCAL_SSID;
    // vban_cfg.password = LOCAL_PASSWORD;
    // vban_cfg.stream_name = "av-receiver-stream";
    // vban_cfg.target_ip = IPAddress{0,0,0,0}; 
    // vban_cfg.throttle_active = true;

    // Decoders
    decoded_out.add(vu);
   // decoded_out.add(vban);
     //decoded_out.add(csv);

    i2s.begin(cfg);
    vu.begin(info);

    // if (!vban.begin(vban_cfg)) {
    //   stop();
    // }

    gui_begin();
}

void loop(){
  copier.copy();

  left_pb.value = vu.value_left();
  right_pb.value = vu.value_right();

  gui_update();
}