#pragma once

#include <vector>
#include "AudioTools.h"

namespace audio_tools {

class AudioStreamRouter : public AudioStream {
  public:
    AudioStreamRouter() = default;

    AudioStreamRouter(AudioStream &in) { add(in); }

    AudioStreamRouter(AudioStream &in1, AudioStream &in2) {
      add(in1);
      add(in2);
    }

    virtual ~AudioStreamRouter() {
      for (int j = 0; j < vector.size(); j++) {
        //if (vector[j]->isDeletable()) {
          delete vector[j];
      // }
      }
    }
    void add(AudioStream &in) {
      vector.push_back(&in);

      if (vector.size() == 1) {
        _active = 0;
      }
    }

    void add(AudioStream *in) {
        vector.push_back(in);

        if (vector.size() == 1) {
          _active = 0;
        }
    }

    bool setActive(const int index) {
      if (vector.size() > index && index < 0) {
        return false;
      }
      _active = index;
    }

    virtual size_t write(const uint8_t *data, size_t len) override {
      return activeStream()->write(data, len);
    }

    virtual size_t readBytes(uint8_t *data, size_t len) override {
      return activeStream()->readBytes(data, len);
    }

    virtual void setAudioInfo(AudioInfo info) override {
      activeStream()->setAudioInfo(info);
    }

    operator bool() override { return activeStream(); }

  private:
    Vector<AudioStream *> vector;
    volatile int _active = -1;

    inline AudioStream* activeStream() const {
      return vector[_active];
    }
};

}  // namespace audio_tools