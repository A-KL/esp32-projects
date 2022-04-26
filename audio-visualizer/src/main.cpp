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

//ES9018 dac = ES9018("Stereo DAC", ES9018::Clock100Mhz, ES9018::Stereo, ES9018::InPhase, ES9018::InPhase, 0x48);

int _selectedAudioSource = 0;
int _selectedAudioTarget = 1;

#include "ui.h"
#include "audio.h"
#include "events.h"
#include "espressif_logo.h"

#include <Wire.h>

bool readI2c(uint8_t address, uint8_t reg, uint8_t& value) {
  Wire.beginTransmission(address);
  Wire.write(byte(reg));
  auto error = Wire.endTransmission();

  if (error) {
    Serial.print("I2C error ");
    Serial.print(address);
    Serial.print(" reg ");
    Serial.print(reg);
    Serial.print(" code: ");
    Serial.print(error);
    
    return false;
  }

  auto to_receive = sizeof(value);
  error = Wire.requestFrom(address, to_receive);

   if (Wire.available() <= to_receive) { 
    value = Wire.read();  
  }

  return true;
}

bool writeI2c(uint8_t address, uint8_t reg, uint8_t value) {
  Wire.beginTransmission(address);
  Wire.write(byte(reg));
  Wire.write(byte(value));
  auto error = Wire.endTransmission();

  if (error) {
    Serial.print("I2C error ");
    Serial.print(address);
    Serial.print(" reg ");
    Serial.print(reg);
    Serial.print(" code: ");
    Serial.print(error);
    
    return false;
  }

  return true;
}

void initDac()
{
  Wire.begin(GPIO_NUM_21, GPIO_NUM_22);

  uint8_t res = 0; 

  if (readI2c(0x48, 0x40, res)){
    Serial.print("0x40 = ");
    Serial.println(res);
  }

  // unsigned long DPLLNum=0;
  // for (auto i=0; i<4; i++){
  //   if (readI2c(0x48, 31 - i, res)){
  //     Serial.print(31 - i);
  //     Serial.print(" = ");
  //     Serial.println(res);

  //     DPLLNum|=res;

  //     if(i != 3) {
  //       DPLLNum<<=8;
  //     }
  //   }
  // }

  //  Serial.println(DPLLNum);
  
  writeI2c(0x48, 0x01, 0xcc);

  if (readI2c(0x48, 0x01, res)){
    Serial.print("0x01 = ");
    Serial.println(res);
  }

}

void setup() {
  Serial.begin(115200);

  canvas.Init(Color::White);
  canvas.SetFont(0, 1);
  canvas.DrawImage(0, 30, 320, 180, espressif_logo_featured);

  initDac();

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