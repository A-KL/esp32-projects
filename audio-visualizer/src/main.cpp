#include <limits.h>
#include <sstream>
#include <Arduino.h>
#include <arduinoFFT.h>

#include "Network.h"
//#include "Radio.h"

#include "Color.h"
#include "Canvas.h"
#include "espressif-logo-featured.h"

#ifdef M5STACK
  #include "M5StackCanvas.h"
  #define TCanvas M5StackCanvas
#endif

#ifdef ESP_WROVER
  #include "TFTCanvas.h"
  #define TCanvas TFTCanvas
#endif

TCanvas canvas;

typedef struct __attribute__((packed)) {
    int16_t left;
    int16_t right;
} AudioFrame;

#include "urls.h"
#include "events.h"
#include "ui.h"
#include "audio.h"

void setup() {
  Serial.begin(115200);

  setupEncoder();

  canvas.Init(Color(255, 255, 255));
  canvas.DrawImage(0, 30, 320, 180, espressif_logo_featured);

  setupWiFi();

  canvas.Clear(Color(0, 0, 0));

  startAnalyzer((void*)&canvas);

  auto is_muted = false;

  while (true)
  {
    auto copier = setupAudio();

    while (true)
    {
      if (encoder_left_.isEncoderButtonClicked(50))
      {
          is_muted = !is_muted;
      }

      if (encoder_left_.encoderChanged())
      {
          float level = encoder_left_.readEncoder();
          Serial.print("Value: ");
          Serial.println(level);
          volume.setFactor(level / 255.0f);
          std::ostringstream temp;
          temp << level;
          label_vol.setText(temp.str().c_str());
      }
      else if (is_muted)
      {
          volume.setFactor(0.0);
      }

      loopAudio(copier);
      
    }
  }
}

void loop() {
}