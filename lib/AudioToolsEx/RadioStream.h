#pragma once

#include <vector>

#include "AudioTools/Communication/AudioHttp.h"
#include "RadioStation.h"

//using namespace arduino;

namespace audio_tools {

class RadioStream : public URLStream {
  public:
    RadioStream(
      const RadioStation* stations, 
      const size_t count, 
      const char* network, const char* password, int readBufferSize = DEFAULT_BUFFER_SIZE) 
      : URLStream(network, password, readBufferSize), _selected_radio(0) {
        setPlaylist(stations, count);
    }

  void setStation(const int i) {
    auto channel = constrain(i, 0, _radios.size());

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
  
  virtual bool begin(int index = -1, MethodID action = GET, const char* reqMime = "", const char* reqData = "") {
    auto& station = _radios[ index != -1 ? index : _selected_radio];
    auto success = URLStream::begin(station.Url, "audio/mp3", action, reqMime, reqData);

    LOGW("Radio: %s (%s) [%s]", station.Name, station.Url, success ? "OK" : "FAILED");
    return success;
  }

  const char* getTitle() {
    return  _radios[_selected_radio].Name;
  }

  private:
    std::vector<RadioStation> _radios;
    int _selected_radio = 0;
};

}  // namespace audio_tools