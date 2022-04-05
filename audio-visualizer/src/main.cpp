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

//#include "espressif-logo-featured.h"

#include "ui.h"
#include "a2dp_spdif.h"

void setup() {
  Serial.begin(115200);

  canvas.Init(Color(255, 255, 255));

  //canvas.DrawImage(0, 30, 320, 180, espressif_logo_featured);

  setupWiFi();

  setupEncoder();

  //setupRadio();

  canvas.Clear(Color(0, 0, 0));

  startAnalyzer((void*)&canvas);

  setupA2DP();
}

void loop() {
  //loopRadio();
  loopA2DP();
}