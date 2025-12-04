#include "AudioTools.h"
#include "AudioTools/Concurrency/RTOS.h"
#include "AudioTools/AudioLibs/AudioRealFFT.h"

#include "VuOutput.h"
#include "gui.h"

AudioInfo info(I2S_SAMPLE_RATE, I2S_CHANNELS, I2S_BPS);

I2SStream i2s;
MultiOutput decoded_out;

VuMeter<int32_t> vu(AUDIO_VU_RATIO);
// VolumeMeter meter_out;
AudioRealFFT fft_out;

StreamCopy copier(decoded_out, i2s);

Task task("write", 5 * 1024, 10, 0);

void fftResult(AudioFFTBase &fft) {
    //fft.frequencyToBin()
    for (auto i = 0; i < spectrum.size(); i++) {
        auto bin_index = i;// ftt_bin_map[i]; 
        auto bin_value = fft.magnitude(bin_index);
       // form.equalizer.bands.setBand(i, sqrt(bin_value)*15);
        spectrum.set_value(i, bin_value * 3);
    }
}

void setup() {
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Warning);

  // TFT
  tft.init();
  tft.setRotation(TFT_ROTATE);
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_BLACK);

  // GUI
  gui_init();
  gui_set_input((int)1);
  gui_progress_bars_update();

  // Audio
  // FFT
  auto tcfg = fft_out.defaultConfig();
  tcfg.length = 1024;
  tcfg.channels = info.channels;
  tcfg.sample_rate = info.sample_rate;
  tcfg.bits_per_sample = info.bits_per_sample;
  //tcfg.window_function = new BufferedWindow(new Hamming());
  //tcfg.window_function = new Hamming();
  tcfg.callback = &fftResult;
  fft_out.begin(tcfg);

  // I2S
  auto cfg = i2s.defaultConfig(RXTX_MODE);
  cfg.copyFrom(info);
  cfg.is_master = false;
  cfg.pin_ws = I2S_WS;
  cfg.pin_bck = I2S_BCK;
  cfg.pin_data = I2S_SD;
  cfg.pin_data_rx = I2S_SDI;
  i2s.begin(cfg);

  // Mixer
  decoded_out.add(vu);
  decoded_out.add(i2s);
  decoded_out.add(fft_out);
  decoded_out.begin(info);

  // VU
  vu.begin(info);
  vu.setAudioInfo(info);

  // Task
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