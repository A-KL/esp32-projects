#pragma once

#include <AiEsp32RotaryEncoder.h>
#include <EasyButton.h>
#include "ES9018K2M.h"

#define VOLUME_PIN_A  35
#define VOLUME_PIN_B  34
#define VOLUME_BUTTON 32

#define MENU_PIN_A  00
#define MENU_PIN_B  02
#define MENU_BUTTON 15

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

AiEsp32RotaryEncoder encoder_left = AiEsp32RotaryEncoder(VOLUME_PIN_A, VOLUME_PIN_B, VOLUME_BUTTON, -1, 4);
AiEsp32RotaryEncoder encoder_right = AiEsp32RotaryEncoder(MENU_PIN_A, MENU_PIN_B, MENU_BUTTON, -1, 4);

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

void setupControls()
{
  encoder_left.begin();
	encoder_left.setup(readEncoderISR);
  encoder_left.setBoundaries(0, 255, false);
  encoder_left.setEncoderValue(200);

  encoder_right.begin();
	encoder_right.setup(readEncoderISR);
  encoder_right.setBoundaries(0, stationsCount-1, true);
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
    initDac();
}

void selectAudio(int dest, int src)
{
  switch (src)
  {
    case 0:
      radio.selectStation(Stations[stationIndex]);
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