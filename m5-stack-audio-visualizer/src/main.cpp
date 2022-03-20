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

#include "UI.h"
#include "main_ui.h"

void setup() {
  Serial.begin(115200);

  canvas.Init(Color(0, 0, 0));

  setupWiFi();
  startAnalyzer((void*)&canvas);

  setupRadio();
}

void loop() {
  loopRadio();
}