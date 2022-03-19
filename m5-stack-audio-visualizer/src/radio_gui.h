#pragma once

typedef struct __attribute__((packed)) {
    const char* Name;
    const char* Url;
} RadioStation;

RadioStation Stations[] { 
  {"Mega Shuffle", "http://jenny.torontocast.com:8134/stream"},

  {"WayUp Radio", "http://188.165.212.154:8478/stream"},
  {"Asia Dream", "https://igor.torontocast.com:1025/;.mp3"},
  {"KPop Radio", "http://streamer.radio.co/s06b196587/listen"},

  {"Classic FM", "http://media-ice.musicradio.com:80/ClassicFMMP3"},
  {"Lite Favorites", "http://naxos.cdnstream.com:80/1255_128"},
  {"MAXXED Out", "http://149.56.195.94:8015/steam"},
  {"SomaFM Xmas", "http://ice2.somafm.com/christmas-128-mp3"}
};

static IRAM_ATTR void enc_cb(void* arg) {
  ESP32Encoder* enc = (ESP32Encoder*) arg;
  Serial.printf("enc cb: count: %d\n", enc->getCount());
}

ESP32Encoder encoder(true, enc_cb);
InternetRadio radio;

void setupRadio()
{
  ESP32Encoder::useInternalWeakPullResistors=UP;
  encoder.attachSingleEdge(4, 5);
  encoder.clearCount();
  encoder.setFilter(1023);

  radio.StartPlaying(Stations[3].Url);
  radio.OnSampleCallback(OnAudioFrameCallback);
}

void updateRadio(void * args)
{
    auto canvas = *(TFTCanvas*)args;

    UIList<RadioStation> stations({ 0, 0, 320, 240 - 23 });

    for(auto i=0; i < sizeof(Stations)/sizeof(Stations[0]); i++)
    {
        stations.Add(Stations[0]);
    }

    panel.Add(stations);
}

void loopRadio()
{
    radio.Loop();
}