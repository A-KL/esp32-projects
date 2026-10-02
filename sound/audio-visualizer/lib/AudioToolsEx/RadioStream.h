#pragma once

#include <vector>
#include "AudioTools/Communication/AudioHttp.h"
#include "RadioStation.h"

namespace audio_tools {

class RadioStream : public URLStream {
  public:
    RadioStream(
      const std::vector<RadioStation> stations,
      const char* network, const char* password, int readBufferSize = DEFAULT_BUFFER_SIZE) 
      : URLStream(network, password, readBufferSize), _radios(stations), _selected_radio(0) 
    { }
  
    virtual bool begin(int index = -1, MethodID action = GET, const char* reqMime = "", const char* reqData = "") {
      if (index != -1) {
        _selected_radio = index;
      }

      auto success = URLStream::begin(getUrl(), "audio/mp3", action, reqMime, reqData);

      LOGW("Radio: %s (%s) [%s]", getTitle(), getUrl(), success ? "OK" : "FAILED");
      
      return success;
    }

    const char* getTitle() const {
      return  _radios[_selected_radio].Name;
    }

    const char* getUrl() const {
      return  _radios[_selected_radio].Url;
    }

    void select(const int index) {
      auto channel = constrain(index, 0, _radios.size());

      if (_selected_radio != channel && active) {
        end();
        begin(_selected_radio);
      }
      _selected_radio = channel;
    }

    void setPlaylist(const RadioStation* stations, const size_t count) {
      _radios.clear();
      _radios.insert(_radios.end(), stations, stations+count);
    }

  private:
    std::vector<RadioStation> _radios;
    int _selected_radio = 0;
};

}  // namespace audio_tools