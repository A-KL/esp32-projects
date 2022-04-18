#include <AiEsp32RotaryEncoder.h>
#include <EasyButton.h>

#define VOLUME_PIN_A  34
#define VOLUME_PIN_B  35
#define VOLUME_BUTTON 32

#define MENU_PIN_A  00
#define MENU_PIN_B  02
#define MENU_BUTTON 15

#define DEBOUNCE_TIME 50

// static void onLeftEncoderChanged(void* arg);
// static void onRightEncoderChanged(void* arg);

// static ESP32Encoder encoder_left;
// static ESP32Encoder encoder_right(false, onRightEncoderChanged);

EasyButton encoder_left_button(VOLUME_BUTTON);
EasyButton encoder_right_button(MENU_BUTTON);

//AiEsp32RotaryEncoder encoder_left_ = AiEsp32RotaryEncoder(VOLUME_PIN_A, VOLUME_PIN_B, VOLUME_BUTTON, -1, 5);


// static void onLeftEncoderChanged(void* arg) {
//   auto count = encoder_left.getCount(); 
//   auto level = count > 255.0 ? 255.0 : count < 0 ? 0 : count;
//   //volume.setFactor((level / 255.0));
//   Serial.printf("Left enc: %f\n", level);
// }

// static void onRightEncoderChanged(void* arg) {
//   auto count = encoder_right.getCount();
//   Serial.printf("Right enc: %d\n", count);
// }

void onLeftEncoderButtonUp()
{
  Serial.println("onLeftEncoderButtonUp");
}

void onRightEncoderButtonUp()
{
  auto muted = !radio.muted();
  radio.setMute(muted);
  form.setIcon(5, muted);

  // Serial.print("Muted: ");
  // Serial.println(muted);
}

void setupControls()
{
  encoder_left_button.begin();
  encoder_left_button.onPressed(onLeftEncoderButtonUp);

  encoder_right_button.begin();
  encoder_right_button.onPressed(onRightEncoderButtonUp);
  // encoder_left_button.onPressedFor(LONG_PRESS_MS, brightnessButton);
  // encoder_left_button.onSequence(3, 2000, startAutoMode);
  // encoder_left_button.onSequence(5, 2000, brightnessOff);

  // encoder_left_.begin();
	// encoder_left_.setup(readEncoderISR);
  // encoder_left_.setBoundaries(0, 255, false);
  // encoder_left_.reset(255);

  // pinMode(VOLUME_BUTTON, INPUT_PULLUP);
  // attachInterrupt(digitalPinToInterrupt(VOLUME_BUTTON), onLeftEncoderButtonUp, RISING );

  // pinMode(MENU_BUTTON, INPUT_PULLUP);
  // attachInterrupt(digitalPinToInterrupt(MENU_BUTTON), onRightEncoderButtonUp, RISING );

  // ESP32Encoder::useInternalWeakPullResistors=UP;

  // encoder_left.attachSingleEdge(VOLUME_PIN_A, VOLUME_PIN_B);
  // encoder_left.setCount(127);
  // encoder_left.setFilter(1023);

  // encoder_right.attachSingleEdge(MENU_PIN_A, MENU_PIN_B);
  // encoder_right.clearCount();
  // encoder_right.setFilter(1023);
}

void loopControls()
{
  encoder_right_button.read();
  encoder_left_button.read();
}