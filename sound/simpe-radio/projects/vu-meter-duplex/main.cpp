#include "AudioTools.h"
#include "AudioTools/Concurrency/RTOS.h"

#include "gui.h"
#include "VuOutput.h"

AudioInfo info(I2S_SAMPLE_RATE, I2S_CHANNELS, I2S_BPS);

I2SStream i2s;
MultiOutput decoded_out;

VuMeter<int32_t> vu(AUDIO_VU_RATIO);
VolumeMeter meter_out;

StreamCopy copier(decoded_out, i2s);

Task task("write", 5 * 1024, 10, 0);

void setup() {
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Warning);

  tft.init();
  tft.setRotation(TFT_ROTATE);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);

  gui_init();
  gui_set_input((int)1);
  gui_progress_bars_update();

  auto cfg = i2s.defaultConfig(RXTX_MODE);
  cfg.copyFrom(info);
  cfg.is_master = false;
  // cfg.i2s_format = I2S_PHILIPS_FORMAT;
  cfg.pin_ws = I2S_WS;
  cfg.pin_bck = I2S_BCK;
  cfg.pin_data = I2S_SD;
  cfg.pin_data_rx = I2S_SDI;
  i2s.begin(cfg);

  decoded_out.add(vu);
  decoded_out.add(i2s);
  // decoded_out.add(csv)
  // decoded_out.add(meter_out);
  decoded_out.begin(info);

  vu.begin(info);
  vu.setAudioInfo(info);

  // csv.begin(info);
  // meter_out.begin(info);

  task.begin([](){gui_progress_bars_update(); delay(10);});
}

void loop() {
  copier.copy();
  left_pb.value = vu.value_left();
  right_pb.value = vu.value_right();

  // left_pb.value = meter_out.volumeRatio(0) * left_pb.max;
  // right_pb.value = meter_out.volumeRatio(1) * right_pb.max;
  // gui_progress_bars_update();
}