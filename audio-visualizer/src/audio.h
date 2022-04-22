#pragma once

RadioStation Stations[] { 
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

const int stationsCount = (sizeof(Stations)/sizeof(RadioStation) - 1);
const int stationIndex = 4;

void onAudioFrameCallback(const AudioFrame& frame)
{
  xQueueSend(audioFrameQueue, &frame, 0);
}

void onStreamChanged(const char *type, const char *value)
{
  if (strcmp(type, "StreamTitle") == 0) {
    form.track.setText(value);
    Serial.println(value);
  }
}

void setupAudio()
{
  radio.SampleCallback(onAudioFrameCallback);
  radio.StreamChanged = onStreamChanged;
}

void selectAudio(int dest, int src)
{
  switch (src)
  {
    case 0:
      form.track.setText(Stations[stationIndex].Name);
      radio.selectStation(Stations[stationIndex]);
      radio.Play(dest, src);

      radio.SampleCallback(onAudioFrameCallback);
      break;

    case 1:
      break;
    
    default:
      break;
  }
}

void loopAudio()
{
  radio.Loop(); 
}