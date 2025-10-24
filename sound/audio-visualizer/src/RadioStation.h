#pragma once

#include <iostream>

struct RadioStation 
{
    const char* Name;
    const char* Url;
    
    friend std::ostream& operator<<(std::ostream& os, const RadioStation& station)
    {
        os << station.Name;
        return os;
    }
};

RadioStation RadioStations[] { 
  {"Local", "http://192.168.1.85:49868/stream/swyh.mp3"},
  {"Asia Dream", "https://igor.torontocast.com:1025/;.mp3"},
  {"KPop Radio", "http://streamer.radio.co/s06b196587/listen"},

  {"Classic FM", "http://media-ice.musicradio.com:80/ClassicFMMP3"},
  {"Lite Favorites", "http://naxos.cdnstream.com:80/1255_128"},
  {"MAXXED Out", "http://149.56.195.94:8015/steam"},
  {"SomaFM Xmas", "http://ice2.somafm.com/christmas-128-mp3"},

  {"SWISS Jazz", "http://stream.srg-ssr.ch/m/rsj/mp3_128"},
  {"Veronica ", "https://www.mp3streams.nl/zender/veronica/stream/11-mp3-128"}
};

static constexpr int RadioStationsCount = (sizeof(RadioStations)/sizeof(RadioStation) - 1);