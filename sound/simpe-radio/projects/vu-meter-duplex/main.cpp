#include "AudioTools.h"
#include "AudioTools/Concurrency/RTOS.h"
#include "AudioTools/AudioLibs/AudioRealFFT.h"

#include "VuOutput.h"
#include "gui.h"

#include "SmoothFilter.h"

AudioInfo info(I2S_SAMPLE_RATE, I2S_CHANNELS, I2S_BPS);

I2SStream i2s;
MultiOutput decoded_out;

VuMeter<int32_t> vu(AUDIO_VU_RATIO);
// VolumeMeter meter_out;
AudioRealFFT fft_out;

SineWaveGenerator<int16_t> sineWave(32000);               // subclass of SoundGenerator with max amplitude of 32000
GeneratedSoundStream<int16_t> gen_in(sineWave);               // Stream generated from sine wave

SmoothFilter<float, 20> filter;

StreamCopy copier(decoded_out, i2s); //gen_in

Task task("write", 5 * 1024, 10, 0);

void fftResult(AudioFFTBase &fft) {
    //fft.frequencyToBin()
    for (auto i = 0; i < spectrum.size(); i++) {
        auto bin_index = i * 2 + 2;// ftt_bin_map[i]; 
        auto bin_value = fft.magnitude(bin_index) * 15;

        filter.add(i, bin_value);
        spectrum.set_value(i, filter.get(i));
    }
    auto d  = fft.result();
    log_w("FFT Result: %d\t%f\t%d", d.frequencyAsInt(), d.magnitude, d.bin);

   // log_e("Bins: %f\t%f\t%f", filter.get(3), filter.get(4) , filter.get(0));
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
  gui_update();

  // Audio
  // FFT
  auto tcfg = fft_out.defaultConfig();
  tcfg.length = 1024;
  tcfg.channels = info.channels;
  tcfg.sample_rate = info.sample_rate;
  tcfg.bits_per_sample = info.bits_per_sample;
  //tcfg.window_function = new BufferedWindow(new Hamming());
  tcfg.window_function = new Hamming();
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

  // Generate
  //sineWave.begin(info, N_G3);
  
  filter.begin(SMOOTHED_AVERAGE, 5);

  // Task
  task.begin([](){_gui_update(); delay(10);});
}

void loop() {
  copier.copy();
  left_pb.value = vu.value_left();
  right_pb.value = vu.value_right();
}