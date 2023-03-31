#include "sbus.h"
#include <M5Stack.h>
#include <Wire.h>
#include <esp_log.h>
#include <Adafruit_INA219.h>
#include "lego_plus_driver.h"
#include "radio.h"
#include "widgets.h"

#ifdef PS3
  #include <Ps3Controller.h>
  #define INIT_CONTROLLER Ps3.begin("b8:27:eb:df:b3:ff")
  #define Ps3.isConnected()
#elif PS4
  #include <PS4Controller.h>
  #define INIT_CONTROLLER PS4.begin("b8:27:eb:df:b3:ff")
  #define PS4.isConnected()
#else XBOX
  #include <XboxSeriesXControllerESP32_asukiaaa.hpp>

  XboxSeriesXControllerESP32_asukiaaa::Core
  xboxController("4c:3b:df:a5:1c:42");

  #define INIT_CONTROLLER xboxController.begin()
  #define CONTROLLER_CONNECTED xboxController.isConnected()
#endif

#define GFXFF 1
//#define FF18  &FreeSans12pt7b
#define CF_OL24 &FreeMonoBold9pt7b

Adafruit_INA219 ina219_output(INA219_ADDRESS);
Adafruit_INA219 ina219_input(INA219_ADDRESS + 1);

bool ina219_output_connected = false;
bool ina219_input_connected = false;
bool motor_driver_connected = false;

bfs::SbusRx sbus_rx(&Serial1);
std::array<int16_t, bfs::SbusRx::NUM_CH()> sbus_data;

const int ch_min_value = 200;
const int ch_max_value = 1800;

const int max_ch = 8;

TFT_eSprite spr = TFT_eSprite(&M5.Lcd);

// GUI
const int margin = 5;

WidgetPanel sbus_panel(margin, margin * 1, WidgetPanel::Large, "sbus");
WidgetPanel ps3_panel(margin, margin * 2 + sbus_panel.Height, WidgetPanel::Small, "ps4", COLOR_DARK_BLUE, COLOR_BLUE);

WidgetPanel nrf42_panel(margin * 2 + sbus_panel.Width, margin, WidgetPanel::Small, "nrf42", COLOR_DARK_RED, COLOR_RED);
WidgetPanel encoders_panel(margin * 2 + sbus_panel.Width, margin * 2 + nrf42_panel.Height, WidgetPanel::Medium, "encoders", COLOR_DARK_MAGENTA, COLOR_MAGENTA);
WidgetPanel motors_panel(margin * 2 + sbus_panel.Width, margin * 3 + encoders_panel.Height + nrf42_panel.Height, WidgetPanel::Small, "motors", COLOR_DARK_GREEN, COLOR_GREEN);

WidgetPanel power_panel(margin * 3 + sbus_panel.Width * 2, margin, WidgetPanel::Large, "power", COLOR_DARK_YELLOW, COLOR_YELLOW);

WidgetList<8> sbus_values(sbus_panel, 0, widget_title_height);
WidgetList<2> ps3_values(ps3_panel, 0, widget_title_height);

WidgetList<2> nrf42_values(nrf42_panel, 0, widget_title_height);
WidgetList<4> encoder_values(encoders_panel, 0, widget_title_height);
WidgetList<2> motors_values(motors_panel, 0, widget_title_height);

WidgetList<3> power_values(power_panel, 0, widget_title_height);

//WidgetListPanel<3> power(margin * 3 + sbus_panel.Width * 2, margin, WidgetPanel::Large, "power", COLOR_DARK_YELLOW, COLOR_YELLOW);

int16_t percentage(int16_t value, int16_t min = ch_min_value, int16_t max = ch_max_value)
{
  return map(value, min, max, 0, 100);
}

int16_t speed(int16_t value, int16_t min = ch_min_value, int16_t max = ch_max_value)
{
  return map(value, min, max, -255, 255);
}

void setup() {

  M5.begin();
  M5.Power.begin();

  Wire.begin();
  Serial.begin(115200);

  xboxController.begin();

  // if (INIT_CONTROLLER)
  // {
  //   Serial.println("Failed to start Controller Host");
  // }

  setupRadio();

  if (!ina219_output.begin()) {
    Serial.println("Failed to find output INA219 chip");
    ina219_output_connected = false;
  }

  if (!ina219_input.begin()) {
    Serial.println("Failed to find input INA219 chip");
    ina219_input_connected = false;
  }

  auto version = readVersion();
  motor_driver_connected = version != 0;
  Serial.printf("Motor Driver ver: %d\r\n", version);

  //Serial.print("BT MAC: ");
  //Serial.println(PS4.Cross .getAddress());

  sbus_rx.Begin(16, 17);

  M5.Lcd.fillScreen(0xff0000);

  //spr.setTextSize(2);
  spr.setFreeFont(CF_OL24);
  spr.setColorDepth(8);
  spr.createSprite(320, 240);
}

void loop() {

  auto left_speed = 0;
  auto right_speed = 0;

  xboxController.onLoop();

  if (CONTROLLER_CONNECTED) {

    //PS3.setPlayer(1);

    ps3_values.setText(0, "ls %d", PS4.data.analog.stick.ly);
    ps3_values.setText(1, "rs %d", PS4.data.analog.stick.ry);

    left_speed = map(PS4.data.analog.stick.ly, -128, 128, -255, 255);
    right_speed = map(PS4.data.analog.stick.ry, 128, -128, 255, -255);
  }
  else
  {
    ps3_values.setText(0, "ls ---");
    ps3_values.setText(1, "rs ---");
  }

  if (sbus_rx.Read())
  {
    sbus_data = sbus_rx.ch();

    spr.setTextColor(WHITE); 

    for (int8_t i = 0; i < max_ch; i++)
    {
      sbus_values.setText(i, "ch%d %d%", i, percentage(sbus_data[i]));

      Serial.print(sbus_data[i]);
      Serial.print("\t");
    }
    /* Display lost frames and failsafe data */
    Serial.print(sbus_rx.lost_frame());
    Serial.print("\t");
    Serial.println(sbus_rx.failsafe());

    left_speed = map(sbus_data[2], ch_min_value, ch_max_value, -255, 255);
    right_speed = map(sbus_data[1], ch_min_value, ch_max_value, 255, -255);
  }
  else
  {
    for (int8_t i = 0; i < max_ch; i++) {
      sbus_values.setText(i, "ch%d ---", i);
    } 
  }

  if (isReceived()) 
  {
    auto ch_left = received.channels[2].value;
    auto ch_right = received.channels[4].value;

    left_speed = map(ch_left, ch_min_value, ch_max_value, 255, -255);
    right_speed = map(ch_right, ch_min_value, ch_max_value, 255, -255);

    nrf42_values.setText(0, "ch0 %d", ch_left);
    nrf42_values.setText(1, "ch1 %d", ch_right);
  }  
  else
  {
    nrf42_values.setText(0, "ch0 0.00");
    nrf42_values.setText(1, "ch1 0.00");
  }

  if (ina219_output_connected && ina219_input_connected) 
  {
    auto shunt_voltage = ina219_output.getShuntVoltage_mV();
    auto bus_voltage = ina219_output.getBusVoltage_V();
    auto current_mA = ina219_output.getCurrent_mA();
    auto power_mW = ina219_output.getPower_mW();
    auto load_voltage = bus_voltage + (shunt_voltage / 1000);

    power_values.setText(0, "%.2f V", bus_voltage);
    power_values.setText(1, "%.2f mA", current_mA);
    power_values.setText(2, "%.2f mW", power_mW);
  }
  else
  {
    power_values.setText(0, "0.00 V");
    power_values.setText(1, "0.00 mA");
    power_values.setText(2, "0.00 mW");
  }

  // Encoder
  for (int8_t i = 0; i < 4; i++) 
  {
    encoder_values.setText(i, "ch%d %d", i, motor_driver_connected ? readEncoder(i) : 0);
  }

  // Motor
  if (motor_driver_connected) 
  {
    MotorRun(1, -left_speed);
    MotorRun(2, -left_speed);

    MotorRun(3, right_speed);
    MotorRun(0, right_speed);
  }

  motors_values.setText(0, "l %d", left_speed);
  motors_values.setText(1, "r %d", right_speed);

  // Update GUI
  spr.fillSprite(TFT_BLACK);

  sbus_panel.render(spr);
  ps3_panel.render(spr);
  nrf42_panel.render(spr);
  encoders_panel.render(spr);
  motors_panel.render(spr);
  power_panel.render(spr);

  sbus_values.render(spr);
  ps3_values.render(spr);
  nrf42_values.render(spr);
  encoder_values.render(spr);
  motors_values.render(spr);
  power_values.render(spr);

  spr.pushSprite(0, 0);
}