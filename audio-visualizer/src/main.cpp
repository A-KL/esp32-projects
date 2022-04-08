#include <limits.h>
#include <Arduino.h>
#include <arduinoFFT.h>
#include <ESP32Encoder.h>

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

#include "espressif-logo-featured.h"

#include "urls.h"
#include "ui.h"
#include "audio.h"
#include "events.h"

void setup() {
  Serial.begin(115200);

  setupEncoder();

  canvas.Init(Color(255, 255, 255));

  canvas.DrawImage(0, 30, 320, 180, espressif_logo_featured);

  setupWiFi();

  canvas.Clear(Color(0, 0, 0));

 startAnalyzer((void*)&canvas);
  
  while (true)
  {
    setupAudio(Stations[stationIndex].Url);
    //setupRadio();

    while (true)
    {
      loopAudio();
      //loopRadio();
    }
  }
  
}

void loop() {
}