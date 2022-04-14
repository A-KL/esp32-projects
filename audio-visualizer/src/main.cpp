#include <limits.h>
#include <sstream>
#include <Arduino.h>
#include "RadioStation.h"

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
int stationIndex = 2;

#include "Network.h"
#include "Radio.h"

#include "Color.h"
#include "Canvas.h"
#include "espressif_logo.h"

#ifdef M5STACK
  #include "M5StackCanvas.h"
  #define TCanvas M5StackCanvas
#endif

#ifdef ESP_WROVER
  #include "TFTCanvas.h"
  #define TCanvas TFTCanvas
#endif

TCanvas canvas;

#include "ui.h"
#include "events.h"

int _selectedAudioSource = 0;
int _selectedAudioTarget = 1;

void setup() {
  Serial.begin(115200);

  canvas.Init(Color(255, 255, 255));
  canvas.DrawImage(0, 30, 320, 180, espressif_logo_featured);

  setupWiFi();

  canvas.Clear(Color(0, 0, 0));

  startAnalyzer((void*)&canvas);

  setupControls();

  while (true)
  {
    setupAudio(_selectedAudioTarget, _selectedAudioSource);
    

    while (true)
    {
      // if (encoder_left_.isEncoderButtonClicked(50))
      // {
      //     is_muted = !is_muted;
      // }

      // if (encoder_left_.encoderChanged())
      // {
      //     auto level = encoder_left_.readEncoder() / 255.0f;

      //     //volume.setFactor(level);

      //     std::ostringstream temp;

      //     temp << (int)(level * 100) << "%";

      //     label_vol.setText(temp.str().c_str());
      // }
      // else if (is_muted)
      // {
      //    // volume.setFactor(0.0);
      // }

      loopAudio();
      loopControls();
    }
  }
}

void loop() {
}