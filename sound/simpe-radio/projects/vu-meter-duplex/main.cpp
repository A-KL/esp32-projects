#include "AudioTools.h"
#include "gui.h"
#include "VuOutput.h"

AudioInfo info(I2S_SAMPLE_RATE, I2S_CHANNELS, I2S_BPS);

I2SStream i2s;
MultiOutput decoded_out;
VuMeter<int16_t> vu(AUDIO_VU_RATIO);

StreamCopy copier(vu, i2s);

void setup() {
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Debug);

  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);

  gui_init();
  gui_set_input((int)1);

  auto cfg = i2s.defaultConfig(RX_MODE);
  cfg.copyFrom(info);
  cfg.is_master = false;
  cfg.i2s_format = I2S_STD_FORMAT;
  cfg.pin_ws = I2S_WS;
  cfg.pin_bck = I2S_BCK;
  cfg.pin_data = I2S_SD;
  cfg.pin_data_rx = I2S_SDO;
  i2s.begin(cfg);

 // decoded_out.add(vu);
 // decoded_out.add(i2s);

  vu.begin(info);
}

void loop(){
  copier.copy();
  left_pb.value = vu.value_left();
  right_pb.value = vu.value_right();
  gui_progress_bars_update();
}