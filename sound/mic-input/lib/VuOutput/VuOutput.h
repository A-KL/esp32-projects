#pragma once

#include "envelope_detector.h"

template <typename T> 
class VuMeter : public AudioOutput 
{
public:
    VuMeter(const float gain = 1, bool active = true) 
    {
        this->_gain = gain;
        this->is_active = active;
    }

    AudioInfo defaultConfig(RxTxMode mode) { return defaultConfig(); }

    AudioInfo defaultConfig() {
        AudioInfo info;
        info.channels = 2;
        info.sample_rate = 44100;
        info.bits_per_sample = sizeof(T) * 8;
        return info;
    }

    bool begin(AudioInfo info) override { return begin(info.channels); }

    /// Starts the processing with the defined number of channels
    bool begin(int channels) {
        TRACED();
        cfg.channels = channels;
        return begin();
    }

    /// (Re)start (e.g. if channels is set in constructor)
    bool begin() override {
      this->is_active = true;
      envelope_init(_right_envelope_context, cfg.sample_rate);
      envelope_init(_left_envelope_context, cfg.sample_rate);
      return true;
    }

    virtual void setAudioInfo(AudioInfo info) override {
        TRACEI();
        this->is_active = true;
        info.logInfo();
        cfg = info;
    };

    /// Writes the data - formatted as CSV -  to the output stream
    virtual size_t write(const uint8_t *data, size_t len) override 
    {
      LOGD("VuMeter::write: %d", (int)len);

      if (!is_active) {
        LOGE("is not active");
        return 0;
      }

      if (len==0) {
        return 0;
      }

      if (cfg.channels == 0) {
        LOGW("Channels not defined: using 2");
        cfg.channels = 2;
      }

      size_t samples_read = len / (sizeof(T) * cfg.channels);

      LOGI("Got %d samples to process", (int)samples_read);

      envelope_calculate_right_left<T>(
        data,
        sizeof(T) * 8,
        _gain,
        samples_read / 10,
        _right_envelope_context, 
        _left_envelope_context);

        LOGD("Results. Left: %f Right: %f", 
          _left_envelope_context.envelope_out,
          _right_envelope_context.envelope_out);

      return len;
    }

    inline float value_left(){
      return _left_envelope_context.envelope_out;
    }

    inline float value_right(){
      return _right_envelope_context.envelope_out;
    }
    
  private:
    audio_envelope_context_t _right_envelope_context;
    audio_envelope_context_t _left_envelope_context;
    float _gain;
};