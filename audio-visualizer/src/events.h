#define VOLUME_PIN_A  34
#define VOLUME_PIN_B  35
#define VOLUME_BUTTON 32

#define MENU_PIN_A  00
#define MENU_PIN_B  02
#define MENU_BUTTON 15

#define DEBOUNCE_TIME 50

static void onLeftEncoderChanged(void* arg);
static void onRightEncoderChanged(void* arg);

static ESP32Encoder encoder_left(true, onLeftEncoderChanged);
static ESP32Encoder encoder_right(true, onRightEncoderChanged);

static void onLeftEncoderChanged(void* arg) {
  auto count = encoder_left.getCount(); 
  auto level = count > 255.0 ? 255.0 : count < 0 ? 0 : count;
  volume.setFactor((level / 255.0));
  Serial.printf("Left enc: %f\n", level);
}

static void onRightEncoderChanged(void* arg) {
  auto count = encoder_right.getCount();
  Serial.printf("Right enc: %d\n", count);
}

static void onLeftEncoderButtonUp()
{
  Serial.println("onLeftEncoderButtonUp");

  // if (stationIndex >= stationsCount)
  // {
  //   stationIndex = 0;
  // }
  // else
  // {
  //   stationIndex++;
  // }

  // Serial.println(stationIndex);

  // radio.Stop();
  // radio.Play(Stations[stationIndex].Url);
}

static void onRightEncoderButtonUp()
{
  Serial.println("onRightEncoderButtonUp");
}

void setupEncoder()
{
  pinMode(VOLUME_BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(VOLUME_BUTTON), onLeftEncoderButtonUp, RISING );

  pinMode(MENU_BUTTON, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(MENU_BUTTON), onRightEncoderButtonUp, RISING );

  ESP32Encoder::useInternalWeakPullResistors=UP;

  encoder_left.attachSingleEdge(VOLUME_PIN_A, VOLUME_PIN_B);
  encoder_left.setCount(127);
  encoder_left.setFilter(1023);

  encoder_right.attachSingleEdge(MENU_PIN_A, MENU_PIN_B);
  encoder_right.clearCount();
  encoder_right.setFilter(1023);
}