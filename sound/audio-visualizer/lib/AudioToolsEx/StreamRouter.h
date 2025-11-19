#pragma once

#include "AudioTools.h"

namespace audio_tools {

class StreamRouter : public Stream {
  public:
    StreamRouter() = default;

    StreamRouter(Stream &in) { add(in); }

    StreamRouter(Stream &in1, Stream &in2) {
      add(in1);
      add(in2);
    }

    virtual ~StreamRouter() {
      for (int j = 0; j < vector.size(); j++) {
        //if (vector[j]->isDeletable()) {
          //delete vector[j];
      // }
      }
    }
    void add(Stream &in) {
      vector.push_back(&in);

      if (vector.size() == 1) {
        _active = 0;
      }
    }

    void add(Stream *in) {
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
      return true;
    }

    virtual size_t write(const uint8_t *data, size_t len) override {
      return activeStream()->write(data, len);
    }

    virtual size_t write(const uint8_t data) override {
      return activeStream()->write(data);
    }

    virtual size_t readBytes(uint8_t *data, size_t len) override {
      return activeStream()->readBytes(data, len);
    }

    virtual int available() override { 
      return activeStream()->available();
    }

    virtual int read() override { 
      return activeStream()->read();
    }

    virtual int peek() override { 
      return activeStream()->peek();
    }

    void setTimeout(size_t timeoutMs) {
      return activeStream()->setTimeout(timeoutMs);
    }

  private:
    Vector<Stream *> vector;
    volatile int _active = -1;

    inline Stream* activeStream() const {
      return vector[_active];
    }
};

}  // namespace audio_tools