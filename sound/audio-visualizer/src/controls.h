#pragma once

#ifdef ARDUINO

#include <AiEsp32RotaryEncoder.h>
#include "RadioStation.h"

static AiEsp32RotaryEncoder encoder_left(MENU_PIN_A, MENU_PIN_B, MENU_BUTTON, -1, 4);
static AiEsp32RotaryEncoder encoder_right(VOLUME_PIN_A, VOLUME_PIN_B, VOLUME_BUTTON, -1, 4);
static TaskHandle_t controls_task_t;

void IRAM_ATTR readEncoderISR()
{
  encoder_left.readEncoder_ISR();
  encoder_right.readEncoder_ISR();
}

#endif

void loopControls();

void read_inputs_task(void * args)
{
  while (true) {
    loopControls();
    delay(20);
  }
}

void setupControls(bool async = false)
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

  if (async)
  {
    xTaskCreatePinnedToCore(
                    read_inputs_task,      /* Task function. */
                    "input_controls_task", /* name of task. */
                    10 * 1024,              /* Stack size of task */
                    NULL,                  /* parameter of the task */
                    0,                     /* priority of the task */
                    &controls_task_t,      /* Task handle to keep track of created task */
                    0);                    /* pin task to core 0 */        
  }
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
      setVolume(value);
      log_e("Volume: %d", value);
	}

	if (encoder_left.isEncoderButtonClicked())
	{
    changeAudioInput();
    log_e("encoder_left");
	}

  if (encoder_right.isEncoderButtonClicked())
	{
      setVolume(0);
      log_e("encoder_right");
	}
#else

	if (lgfx::v1::gpio_in(MENU_BUTTON) == 0) {
    changeAudioInput();
    lgfx::delayMicroseconds(100000);
	}

  if (lgfx::v1::gpio_in(VOLUME_BUTTON) == 0) {
      setVolume(0);
      lgfx::delayMicroseconds(100000);
	}
  
#endif
}