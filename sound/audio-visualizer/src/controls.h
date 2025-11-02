#pragma once

#include <AiEsp32RotaryEncoder.h>
#include "RadioStation.h"

#define VOLUME_PIN_A  35
#define VOLUME_PIN_B  34
#define VOLUME_BUTTON 32

#define MENU_PIN_A  00
#define MENU_PIN_B  02
#define MENU_BUTTON 15

static AiEsp32RotaryEncoder encoder_left(VOLUME_PIN_A, VOLUME_PIN_B, VOLUME_BUTTON, -1, 4);
static AiEsp32RotaryEncoder encoder_right(MENU_PIN_A, MENU_PIN_B, MENU_BUTTON, -1, 4);

void IRAM_ATTR readEncoderISR()
{
	encoder_left.readEncoder_ISR();
  encoder_right.readEncoder_ISR();
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

	}

  if (encoder_right.encoderChanged())
	{
      auto value = encoder_right.readEncoder();
      auto dbs = (int)(value* 0.5 - 127.5);
      log_w("Volume: %d", dbs);
      // Serial.println(dbs);
      // volumeDac(value);
      form.volume.setTextF("%ddb", dbs);
	}

	if (encoder_left.isEncoderButtonClicked())
	{
    // auto muted = !radio.muted();
    // radio.setMute(muted);
    // form.setIcon(5, muted);
	}

  if (encoder_right.isEncoderButtonClicked())
	{

	}  
}