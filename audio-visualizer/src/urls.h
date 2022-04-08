struct RadioStation 
{
    const char* Name;
    const char* Url;

    friend std::ostream& operator<<(std::ostream& os, const RadioStation& station);
};

RadioStation Stations[] { 
  {"Asia Dream", "https://igor.torontocast.com:1025/;.mp3"},
  {"KPop Radio", "http://streamer.radio.co/s06b196587/listen"},

  {"Classic FM", "http://media-ice.musicradio.com:80/ClassicFMMP3"},
  {"Lite Favorites", "http://naxos.cdnstream.com:80/1255_128"},
  {"MAXXED Out", "http://149.56.195.94:8015/steam"},
  {"SomaFM Xmas", "http://ice2.somafm.com/christmas-128-mp3"},

  {"SWISS Jazz", "http://stream.srg-ssr.ch/m/rsj/mp3_128"}
};

const int stationsCount = (sizeof(Stations)/sizeof(RadioStation) - 1);
int stationIndex = 2;