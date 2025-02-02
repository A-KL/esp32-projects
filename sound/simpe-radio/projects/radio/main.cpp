#include <OneButton.h>
//#include <AiEsp32RotaryEncoder.h>

// #include "Creds.h"
// #include "AudioTools.h"
// #include "AudioTools/AudioCodecs/CodecMP3Helix.h"

#include "gui.h"
#include "audio.h"

// AiEsp32RotaryEncoder encoder_left = AiEsp32RotaryEncoder(VOLUME_PIN_A, VOLUME_PIN_B, VOLUME_BUTTON, -1, 4);
// AiEsp32RotaryEncoder encoder_right = AiEsp32RotaryEncoder(MENU_PIN_A, MENU_PIN_B, MENU_BUTTON, -1, 4);
// OneButton left_button = OneButton(PIN_BUTTON_1, true, true);
// OneButton right_button = OneButton(PIN_BUTTON_2, true, true);

void buttons_init()
{
  // left_button.attachClick([]() { stop_playing(); });
  // right_button.attachClick([]() { next_station(); });
}

void setup(){
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Warning);

  //pinMode(PIN_LED, OUTPUT);

  gui_init();
  buttons_init();
  audio_init();

  gui_run(0);

  start_playing();
}

void loop()
{
  audio_loop();

  // left_button.tick();
  // right_button.tick();
}