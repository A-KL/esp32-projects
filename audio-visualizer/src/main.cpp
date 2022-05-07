#include <limits.h>
#include <sstream>
#include <Arduino.h>
#include "Network.h"
#include "Color.h"
#include "Canvas.h"
#include "MainForm.h"
#include "AudioFrame.h"
#include "RadioStation.h"
#include "Radio.h"
#include "AdcAudioDevice.h"

#ifdef M5STACK
  #include "M5StackCanvas.h"
  #define TCanvas M5StackCanvas
#endif

#ifdef ESP_WROVER
  #include "TFTCanvas.h"
  #define TCanvas TFTCanvas
#endif

TCanvas canvas;
MainForm form({ 0, 0, 320, 240 });

#define SAMPLES 512

double vReal_l[SAMPLES];
double vReal_r[SAMPLES];

double vImag_l[SAMPLES];
double vImag_r[SAMPLES];

unsigned int samplig_rate = 44100;

InternetRadio radio;
AdcAudioDevice adc((float*)vReal_l, (float*)vImag_l, SAMPLES, samplig_rate);

int _selectedAudioSource = 0;
int _selectedAudioTarget = 1;

#include "ui.h"
#include "audio.h"
#include "events.h"
#include "espressif_logo.h"

void setup() {
  Serial.begin(115200);

  canvas.Init(Color::White);
  canvas.SetFont(0, 1);
  canvas.DrawImage(0, 30, 320, 180, espressif_logo_featured);

  setupWiFi();
  setupControls();
  setupAudio();

  canvas.Clear(Color::Black);

  startUI((void*)&canvas);

  while (true)
  {
    selectAudio(_selectedAudioTarget, _selectedAudioSource);

    form.setIcon(_selectedAudioTarget, 1);
    form.setIcon(_selectedAudioSource + 2, 1);

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