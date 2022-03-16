#include <limits.h>
#include <Arduino.h>
#include <arduinoFFT.h>

#include "Network.h"
#include "Radio.h"

#include "Color.h"
#include "Canvas.h"

#ifdef M5STACK
  #include "M5StackCanvas.h"
  M5StackCanvas canvas;
#endif

#ifdef ESP_WROVER
  #include "TFTCanvas.h"
  TFTCanvas canvas;
#endif

#include "UI.h"
#include "main_ui.h"

// ---------------------------------------------------

const char* urls[] = {
  "http://jenny.torontocast.com:8134/stream",
  
  "http://188.165.212.154:8478/stream",
  "https://igor.torontocast.com:1025/;.mp3",
  "http://streamer.radio.co/s06b196587/listen",
   
  "http://media-ice.musicradio.com:80/ClassicFMMP3",
  "http://naxos.cdnstream.com:80/1255_128",
  "http://149.56.195.94:8015/steam",
  "http://ice2.somafm.com/christmas-128-mp3"
};

const char* station[] = {
  "Mega Shuffle",
 
  "WayUp Radio",
  "Asia Dream",
  "KPop Radio",
  
  "Classic FM",
  "Lite Favorites",
  "MAXXED Out",
  "SomaFM Xmas"
};

const int stations_count = sizeof(urls)/sizeof(char *);

// ---------------------------------------------------

InternetRadio radio;

void setup() {
  Serial.begin(115200);

  canvas.Init(Color(0, 0, 0));

  StartWifi();
  StartAudioUI((void*)&canvas);

  radio.StartPlaying(urls[3]);
  radio.OnSampleCallback(OnAudioFrameCallback);
}

void loop() {
  radio.Loop();
}