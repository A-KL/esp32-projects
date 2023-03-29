#include "sbus.h"
#include <M5Stack.h>
#include <Wire.h>
#include <esp_log.h>
#include <Adafruit_INA219.h>
#include <Ps3Controller.h>
#include "lego_plus_driver.h"
#include "radio.h"
#include "widgets.h"

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

const int min_ch_value = 200;
const int max_ch_value = 1800;

const int max_ch = 8;

TFT_eSprite spr = TFT_eSprite(&M5.Lcd);

// GUI
const int margin = 5;

WidgetPanel sbus_panel(margin, margin * 1, WidgetPanel::Large, "sbus");
WidgetPanel ps3_panel(margin, margin * 2 + sbus_panel.Height, WidgetPanel::Small, "ps3", COLOR_DARK_BLUE, COLOR_BLUE);

WidgetPanel nrf42_panel(margin * 2 + sbus_panel.Width, margin, WidgetPanel::Small, "nrf42", COLOR_DARK_RED, COLOR_RED);
WidgetPanel encoders_panel(margin * 2 + sbus_panel.Width, margin * 2 + nrf42_panel.Height, WidgetPanel::Medium, "encoders", COLOR_DARK_MAGENTA, COLOR_MAGENTA);
WidgetPanel motors_panel(margin * 2 + sbus_panel.Width, margin * 3 + encoders_panel.Height + nrf42_panel.Height, WidgetPanel::Small, "motors", COLOR_DARK_GREEN, COLOR_GREEN);

WidgetPanel power_panel(margin * 3 + sbus_panel.Width * 2, margin, WidgetPanel::Large, "power", COLOR_DARK_YELLOW, COLOR_YELLOW);

WidgetList<8> sbus_values(sbus_panel.Left, sbus_panel.Top + widget_title_height);
WidgetList<2> ps3_values(ps3_panel.Left, ps3_panel.Top + widget_title_height);

WidgetList<2> nrf42_values(nrf42_panel.Left, nrf42_panel.Top + widget_title_height);
WidgetList<4> encoder_values(encoders_panel.Left, encoders_panel.Top + widget_title_height);
WidgetList<2> motors_values(motors_panel.Left, motors_panel.Top + widget_title_height);

WidgetList<3> power_values(power_panel.Left, power_panel.Top + widget_title_height);

int16_t perc(int16_t value)
{
  return map(value, min_ch_value, max_ch_value, 0, 100);
}

uint16_t color565( const unsigned long rgb) {

  uint16_t R = (rgb >> 16) & 0xFF;
  uint16_t G = (rgb >>  8) & 0xFF;
  uint16_t B = (rgb      ) & 0xFF;

  uint16_t ret  = (R & 0xF8) << 8;  // 5 bits
           ret |= (G & 0xFC) << 3;  // 6 bits
           ret |= (B & 0xF8) >> 3;  // 5 bits
       
  return (ret);
}

void setup() {

  M5.begin();
  M5.Power.begin();

  Wire.begin();
  Serial.begin(115200);

  if (!Ps3.begin("b8:27:eb:df:b3:ff"))
  {
    Serial.println("Failed to start PS3 Controller Host");
  }

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

  Serial.print("BT MAC: ");
  Serial.println(Ps3.getAddress());

  sbus_rx.Begin(16, 17);

  M5.Lcd.fillScreen(0xff0000);

  //spr.setTextSize(2);
  spr.setFreeFont(CF_OL24);
  spr.setColorDepth(8);
  spr.createSprite(320, 240);
}

void gui_text(TFT_eSprite& canvas, int16_t x, int16_t y, const char* text, uint32_t color) {

  canvas.setTextColor(color565(color)); 
  canvas.setCursor(x, y);
  canvas.print(text);
}

void loop() {

  spr.fillSprite(TFT_BLACK);

  sbus_panel.Render(spr);
  ps3_panel.Render(spr);

  nrf42_panel.Render(spr);
  encoders_panel.Render(spr);
  motors_panel.Render(spr);

  power_panel.Render(spr);

  auto left_speed = 0;
  auto right_speed = 0;

  if (Ps3.isConnected()) {

    Ps3.setPlayer(1);

    // spr.setTextColor(ORANGE);
    // spr.setCursor(180, 0);
    // spr.printf("PS3: %d\n", Ps3.data.analog.stick.ly);
    // spr.setCursor(180, 20);
    // spr.printf("PS3: %d\n", Ps3.data.analog.stick.ry);

    ps3_values.SetText(0, "ls %d", Ps3.data.analog.stick.ly);
    ps3_values.SetText(1, "rs %d", Ps3.data.analog.stick.ry);
  }
  else
  {
    ps3_values.SetText(0, "ls 0.00");
    ps3_values.SetText(1, "rs 0.00");
    //gui_text(spr, margin + text_margin_x, margin + widget_l_height + margin + widget_title_height + text_margin_y * 1, "ls 0.00", COLOR_LIGHT_GRAY);
    //gui_text(spr, margin + text_margin_x, margin + widget_l_height + margin + + widget_title_height + text_margin_y * 2, "rs 0.00", COLOR_LIGHT_GRAY);
  }

  if (sbus_rx.Read())
  {
    sbus_data = sbus_rx.ch();

    spr.setTextColor(WHITE); 

    for (int8_t i = 0; i < max_ch; i++) {
      sbus_values.SetText(i, "ch%d %d%", i, perc(sbus_data[i]));

      // spr.setCursor(0, (i + 1) * 20);
      // spr.printf("CH%d: %d\n", i, perc(sbus_data[i]));

      Serial.print(sbus_data[i]);
      Serial.print("\t");
    }

    /* Display lost frames and failsafe data */
    Serial.print(sbus_rx.lost_frame());
    Serial.print("\t");
    Serial.println(sbus_rx.failsafe());

    left_speed = map(sbus_data[2], min_ch_value, max_ch_value, -255, 255);
    right_speed = map(sbus_data[1], min_ch_value, max_ch_value, 255, -255);
  }
  else
  {
    for (int8_t i = 0; i < max_ch; i++) {
      sbus_values.SetText(i, "ch%d 0.00", i);
    } 
  }

  if (isReceived()) {
    auto ch_left = received.channels[2].value;
    auto ch_right = received.channels[4].value;

    left_speed = map(ch_left, min_ch_value, max_ch_value, 255, -255);
    right_speed = map(ch_right, min_ch_value, max_ch_value, 255, -255);

    // spr.setTextColor(WHITE); 
    // spr.setCursor(150, 210);
    // spr.printf("RF0: %d\n", ch_left);
    // spr.setCursor(150, 230);
    // spr.printf("RF1: %d\n", ch_right);

    nrf42_values.SetText(0, "ch0 %d", ch_left);
    nrf42_values.SetText(1, "ch1 %d", ch_right);
  }  
  else
  {
    nrf42_values.SetText(0, "ch0 0.00");
    nrf42_values.SetText(1, "ch1 0.00");
    //gui_text(spr, margin + margin + text_margin_x + widget_width, margin + text_margin_y * 1 + widget_title_height, "ch0 0.00", COLOR_LIGHT_GRAY);
    //gui_text(spr, margin + margin + text_margin_x + widget_width, margin + text_margin_y * 2 + widget_title_height, "ch1 0.00", COLOR_LIGHT_GRAY);
  }

  if (ina219_output_connected && ina219_input_connected) {

    auto shunt_voltage = ina219_output.getShuntVoltage_mV();
    auto bus_voltage = ina219_output.getBusVoltage_V();
    auto current_mA = ina219_output.getCurrent_mA();
    auto power_mW = ina219_output.getPower_mW();
    auto load_voltage = bus_voltage + (shunt_voltage / 1000);

    // spr.setTextColor(ORANGE); 
    // spr.setCursor(150, 110);
    // spr.printf("V: %.2fV\n", bus_voltage);
    // spr.setCursor(150, 130);
    // spr.printf("I: %.2fmA\n", current_mA);

    power_values.SetText(0, "%.2f V", bus_voltage);
    power_values.SetText(1, "%.2f mA", current_mA);
    power_values.SetText(2, "%.2f mW", power_mW);
  }
  else
  {
    // gui_text(spr, 
    // margin + widget_width + margin + widget_width + margin + text_margin_x, 
    // margin + widget_title_height + text_margin_y * 1, 
    // "0.00 V", COLOR_LIGHT_GRAY);

    // gui_text(spr, 
    // margin + widget_width + margin + widget_width + margin + text_margin_x, 
    // margin + widget_title_height + text_margin_y * 2, 
    // "0.00 mA", COLOR_LIGHT_GRAY);

    // gui_text(spr, 
    // margin + widget_width + margin + widget_width + margin + text_margin_x, 
    // margin + widget_title_height + text_margin_y * 3, 
    // "0.00 V", COLOR_LIGHT_GRAY);

    // gui_text(spr, 
    // margin + widget_width + margin + widget_width + margin + text_margin_x, 
    // margin + widget_title_height + text_margin_y * 4, 
    // "0.00 mA", COLOR_LIGHT_GRAY);

    power_values.SetText(0, "0.00 V");
    power_values.SetText(1, "0.00 mA");
    power_values.SetText(2, "0.00 mW");
  }

  if (motor_driver_connected) {
    // spr.setTextColor(GREEN);

    for (int8_t i = 0; i < 4; i++) {
      // spr.setCursor(150, (i + 1) * 20 ); //+ 8 * 15
      // spr.printf("Enc%d: %d\n", i, readEncoder(i));

      encoder_values.SetText(i, "ch%d %d", i, readEncoder(i));
    }

    // spr.setTextColor(RED); 
    // spr.setCursor(0, (10 * 20));
    // spr.printf("Left: %d\n", left_speed);
    // spr.setCursor(0, (11 * 20));
    // spr.printf("Right: %d\n", right_speed);

    motors_values.SetText(0, "l %d", left_speed);
    motors_values.SetText(1, "r %d", right_speed);

    MotorRun(1, -left_speed);
    MotorRun(2, -left_speed);

    MotorRun(3, right_speed);
    MotorRun(0, right_speed);
  }
  else
  {
    // gui_text(spr, 
    //   margin + widget_width + margin + text_margin_x, 
    //   margin + widget_s_height + margin + widget_m_height + margin + widget_title_height + text_margin_y * 1, 
    //   "l 0.00%", COLOR_LIGHT_GRAY);

    // gui_text(spr, 
    //   margin + widget_width + margin + text_margin_x, 
    //   margin + widget_s_height + margin + widget_m_height + margin + widget_title_height + text_margin_y * 2, 
    //   "r 0.00%", COLOR_LIGHT_GRAY);

    motors_values.SetText(0, "l 0.00");
    motors_values.SetText(1, "r 0.00");

    for (int8_t i = 0; i < 4; i++) {
      encoder_values.SetText(i, "ch%d 0.00", i);
    }

    // gui_text(spr, 
    //   margin + widget_width + margin + text_margin_x, 
    //   margin + widget_s_height  + margin + widget_title_height + text_margin_y * 1, 
    //   "ch0 0.00", COLOR_LIGHT_GRAY);

    // gui_text(spr, 
    //   margin + widget_width + margin + text_margin_x, 
    //   margin + widget_s_height + margin + widget_title_height + text_margin_y * 2, 
    //   "ch1 0.00", COLOR_LIGHT_GRAY);

    // gui_text(spr, 
    //   margin + widget_width + margin + text_margin_x, 
    //   margin + widget_s_height  + margin + widget_title_height + text_margin_y * 3, 
    //   "ch2 0.00", COLOR_LIGHT_GRAY);

    // gui_text(spr, 
    //   margin + widget_width + margin + text_margin_x, 
    //   margin + widget_s_height + margin + widget_title_height + text_margin_y * 4, 
    //   "ch3 0.00", COLOR_LIGHT_GRAY);
  }

  sbus_values.Render(spr);
  ps3_values.Render(spr);
  nrf42_values.Render(spr);
  encoder_values.Render(spr);
  motors_values.Render(spr);
  power_values.Render(spr);

  spr.pushSprite(0, 0);
}