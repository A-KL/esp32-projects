#pragma once

#include <AiEsp32RotaryEncoder.h>
//#include <EasyButton.h>
#include "ES9018K2M.h"
#include "Radio.h"

#define VOLUME_PIN_A  35
#define VOLUME_PIN_B  34
#define VOLUME_BUTTON 32

#define MENU_PIN_A  00
#define MENU_PIN_B  02
#define MENU_BUTTON 15

const int stationIndex = 4;

AiEsp32RotaryEncoder encoder_left = AiEsp32RotaryEncoder(VOLUME_PIN_A, VOLUME_PIN_B, VOLUME_BUTTON, -1, 4);
AiEsp32RotaryEncoder encoder_right = AiEsp32RotaryEncoder(MENU_PIN_A, MENU_PIN_B, MENU_BUTTON, -1, 4);

InternetRadio radio;

void IRAM_ATTR readEncoderISR()
{
	encoder_left.readEncoder_ISR();
  encoder_right.readEncoder_ISR();
}

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

void log_init()
{
  Serial.begin(115200);
}

void setupControls()
{
  encoder_left.begin();
	encoder_left.setup(readEncoderISR);
  encoder_left.setBoundaries(0, 255, false);
  encoder_left.setEncoderValue(200);

  encoder_right.begin();
	encoder_right.setup(readEncoderISR);
  encoder_right.setBoundaries(0, RadioStationsCount-1, true);
  encoder_right.setEncoderValue(0);
}

void loopControls()
{
	if (encoder_left.encoderChanged())
	{
    auto value = encoder_left.readEncoder();
    auto dbs = (value* 0.5 - 127.5);
		Serial.println(dbs);
    volumeDac(value);
    form.volume.setTextF("%ddb", dbs);
	}

	if (encoder_left.isEncoderButtonClicked())
	{
    auto muted = !radio.muted();
    radio.setMute(muted);
    form.setIcon(5, muted);
	}

  if (encoder_right.isEncoderButtonClicked())
	{

	}  
}

void setupAudio()
{ 
    //initDac();
}

void selectAudio(int dest, int src)
{
  switch (src)
  {
    case 0:
      radio.selectStation(RadioStations[stationIndex].Url);
      radio.StreamChanged = onStreamChanged;
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