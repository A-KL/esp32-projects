#pragma once

#include "AudioTools.h"

class NumberFormatConverterStreamV2 : public NumberFormatConverterStream {
 public:
  NumberFormatConverterStreamV2() = default;
  NumberFormatConverterStreamV2(Stream& stream) {
    setStream(stream);
  }
  NumberFormatConverterStreamV2(AudioStream& stream) {
    setStream(stream);
  }
  NumberFormatConverterStreamV2(Print& print) {
    setOutput(print);
  }
  NumberFormatConverterStreamV2(AudioOutput& print) {
    setOutput(print);
  }

  void setAudioInfo(AudioInfo newInfo) override {
    source_bits = newInfo.bits_per_sample;
    NumberFormatConverterStream::setAudioInfo(newInfo);
  }

  bool begin(AudioInfo info, int toBits, float gain = 1.0f) {
    source_bits = info.bits_per_sample;
    target_bits = toBits;
    conversion_gain = gain;
    return NumberFormatConverterStream::begin(info, toBits, gain);
  }

  bool begin(AudioInfo info, AudioInfo to, float gain = 1.0f) {
    source_bits = info.bits_per_sample;
    target_bits = to.bits_per_sample;
    conversion_gain = gain;
    return NumberFormatConverterStream::begin(info, to, gain);
  }

  void setToBits(uint8_t bits) {
    target_bits = bits;
    NumberFormatConverterStream::setToBits(bits);
  }

  void setOutput(Print& print) override {
    ReformatBaseStream::setOutput(print);
    rebind();
  }

  void setOutput(AudioOutput& print) override {
    ReformatBaseStream::setOutput(print);
    rebind();
  }

  void setStream(Stream& stream) override {
    ReformatBaseStream::setStream(stream);
    rebind();
  }

  void setStream(AudioStream& stream) override {
    ReformatBaseStream::setStream(stream);
    rebind();
  }

  size_t write(const uint8_t* data, size_t len) override {
    if (source_bits == 16 && target_bits == 32) {
      if (len == 0 || getPrint() == nullptr) return 0;

      const size_t frame_count = len / sizeof(int16_t);
      int32_t converted[512];
      size_t written_frames = 0;
      const int16_t* source = reinterpret_cast<const int16_t*>(data);

      for (size_t offset = 0; offset < frame_count;) {
        const size_t count = min(frame_count - offset,
                                 static_cast<size_t>(512));
        for (size_t i = 0; i < count; i++) {
          converted[i] = static_cast<int32_t>(source[offset + i]) * 65536;
        }
        const size_t written = getPrint()->write(
            reinterpret_cast<const uint8_t*>(converted), count * sizeof(int32_t));
        if (written == 0) break;
        written_frames += written / sizeof(int32_t);
        offset += count;
      }

      return written_frames == frame_count ? len : written_frames * sizeof(int16_t);
    }

    return NumberFormatConverterStream::write(data, len);
  }

 private:
  int source_bits = 16;
  int target_bits = 32;
  float conversion_gain = 1.0f;

  void rebind() {
    if (source_bits > 0 && target_bits > 0) {
      NumberFormatConverterStream::begin(source_bits, target_bits,
                                         conversion_gain);
    }
  }
};
