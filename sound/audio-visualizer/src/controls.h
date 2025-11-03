#pragma once

#ifdef ARDUINO

#include <AiEsp32RotaryEncoder.h>
#include "RadioStation.h"

#define MENU_PIN_A  35
#define MENU_PIN_B  34
#define MENU_BUTTON 32

#define VOLUME_PIN_A  04
#define VOLUME_PIN_B  02
#define VOLUME_BUTTON 15

static AiEsp32RotaryEncoder encoder_left(MENU_PIN_A, MENU_PIN_B, MENU_BUTTON, -1, 4);
static AiEsp32RotaryEncoder encoder_right(VOLUME_PIN_A, VOLUME_PIN_B, VOLUME_BUTTON, -1, 4);

void IRAM_ATTR readEncoderISR() //ARDUINO_ISR_ATTR
{
  encoder_left.readEncoder_ISR();
  encoder_right.readEncoder_ISR();
}

#endif

void setupControls()
{
#ifdef ARDUINO
  encoder_left.begin();
	encoder_left.setup(readEncoderISR);
  encoder_left.setBoundaries(0, RadioStationsCount-1, true);
  encoder_left.setEncoderValue(0);
  encoder_left.disableAcceleration();
  encoder_left.enable();

  encoder_right.begin();
	encoder_right.setup(readEncoderISR);
  encoder_right.setBoundaries(0, 255, false);
  encoder_right.setEncoderValue(200);
  encoder_right.disableAcceleration();
  encoder_right.enable();
#endif
}

void loopControls()
{
#ifdef ARDUINO
	if (encoder_left.encoderChanged())
	{
    auto value = encoder_left.readEncoder();
    log_e("Channel: %i", value);
	}

  if (encoder_right.encoderChanged())
	{
      auto value = encoder_right.readEncoder();
      log_e("Volume: %d", value);
      auto dbs = (int)(value* 0.5 - 127.5);
      // volumeDac(value);
      form.volume.setTextF("%ddb", dbs);
	}

	if (encoder_left.isEncoderButtonClicked())
	{
    // auto muted = !radio.muted();
    // radio.setMute(muted);
    // form.setIcon(5, muted);
    log_e("encoder_left");
	}

  if (encoder_right.isEncoderButtonClicked())
	{
      log_e("encoder_right");
	}
#endif
}