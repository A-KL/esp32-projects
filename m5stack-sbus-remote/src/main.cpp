#include "sbus.h"
#include <M5Stack.h>
#include <Wire.h>
#include <esp_log.h>
#include <Adafruit_INA219.h>
#include <Ps3Controller.h>
#include "lego_plus_driver.h"
#include "radio.h"

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

const int COLOR_LIGHTGRAY = 0xBDBDBD;

const int COLOR_DARKGRAY = 0x202020; // change
const int COLOR_GRAY = 0x464646; 

const int COLOR_DARKRED = 0x3c0000;
const int COLOR_RED = 0x730000;

const int COLOR_DARKMAGENTA = 0x3e0030; // change
const int COLOR_MAGENTA = 0x710054;

const int COLOR_DARKGREEN = 0x092b00;
const int COLOR_GREEN = 0x155804;

const int COLOR_DARKBLUE = 0x00252F; // change
const int COLOR_BLUE = 0x004056;

const int COLOR_DARKYELLOW = 0x804500;
const int COLOR_YELLOW = 0xAF6700;

TFT_eSprite spr = TFT_eSprite(&M5.Lcd);

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
       
  return( ret);
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

void gui_panel(TFT_eSprite& canvas, int16_t x, int16_t y, int16_t height, const char* title, uint32_t color1, uint32_t color2, uint32_t text_color) {

  const int corner_radius = 0;
  const int widget_width = 100;
  const int widget_title_height = 16;

  const int text_margin_y = 12;
  const int text_margin_x = 5;

  const int widget_s_height = 55;
  const int widget_m_height = 110;
  const int widget_l_height = 170;
  const int widget_xl_height = 230;

  canvas.fillRoundRect(x, y, widget_width, height, corner_radius, color565(color1));
  canvas.fillRoundRect(x, y, widget_width, widget_title_height, corner_radius, color565(color2));

  gui_text(canvas, x + text_margin_x, y + text_margin_y, title, text_color);
}

void gui(TFT_eSprite& canvas) {

  const int margin = 5;

  const int widget_width = 100;

  const int widget_s_height = 55;
  const int widget_m_height = 110;
  const int widget_l_height = 170;
  const int widget_xl_height = 230;

  // Left
  int x = margin;
  int y = margin;

  gui_panel(canvas, x, y, widget_l_height, "sbus", COLOR_DARKGRAY, COLOR_GRAY, COLOR_LIGHTGRAY);

  y += widget_l_height;
  y += margin;

  gui_panel(canvas, x, y, widget_s_height, "ps3", COLOR_DARKBLUE, COLOR_BLUE, COLOR_LIGHTGRAY);

  // Center

  x = margin;
  y = margin;

  x += widget_width;
  x += margin;

  gui_panel(canvas, x, y, widget_s_height, "nrf42", COLOR_DARKRED, COLOR_RED, COLOR_LIGHTGRAY);

  y += widget_s_height;
  y += margin;

  gui_panel(canvas, x, y, widget_m_height, "encoders", COLOR_DARKMAGENTA, COLOR_MAGENTA, COLOR_LIGHTGRAY);

  y += widget_m_height;
  y += margin;

  gui_panel(canvas, x, y, widget_s_height, "motors", COLOR_DARKGREEN, COLOR_GREEN, COLOR_LIGHTGRAY);

  // Right

  x = margin;
  y = margin;

  x += widget_width;
  x += margin;

  x += widget_width;
  x += margin;

  gui_panel(canvas, x, y, widget_l_height, "power", COLOR_DARKYELLOW, COLOR_YELLOW, COLOR_LIGHTGRAY);
}

void loop() {

  spr.fillSprite(TFT_BLACK);

  gui(spr);

  auto left_speed = 0;
  auto right_speed = 0;

  const int margin = 5;

  const int widget_width = 100;
  const int widget_title_height = 16;

  const int text_margin_y = 14;
  const int text_margin_x = 5;

  const int widget_s_height = 55;
  const int widget_m_height = 110;
  const int widget_l_height = 170;
  const int widget_xl_height = 230;

  if (Ps3.isConnected()) {

    Ps3.setPlayer(1);

    spr.setTextColor(ORANGE);
    spr.setCursor(180, 0);
    spr.printf("PS3: %d\n", Ps3.data.analog.stick.ly);
    spr.setCursor(180, 20);
    spr.printf("PS3: %d\n", Ps3.data.analog.stick.ry);
  }
  else
  {
    gui_text(spr, margin + text_margin_x, margin + widget_l_height + margin + widget_title_height + text_margin_y * 1, "ls 0.00", COLOR_LIGHTGRAY);
    gui_text(spr, margin + text_margin_x, margin + widget_l_height + margin + + widget_title_height + text_margin_y * 2, "rs 0.00", COLOR_LIGHTGRAY);
  }

  auto sbus_enabled = sbus_rx.Read();

  if (sbus_enabled)
  {
    sbus_data = sbus_rx.ch();

    spr.setTextColor(WHITE); 

    for (int8_t i = 0; i < max_ch; i++) {
      spr.setCursor(0, (i + 1) * 20);
      spr.printf("CH%d: %d\n", i, perc(sbus_data[i]));

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
      gui_text(spr, margin + text_margin_x, margin + widget_title_height + text_margin_y * (1 + i) + 2, "ch0 0.00", COLOR_LIGHTGRAY);
    } 
  }

  if (isReceived()) {
    auto ch_left = received.channels[2].value;
    auto ch_right = received.channels[4].value;

    left_speed = map(ch_left, min_ch_value, max_ch_value, 255, -255);
    right_speed = map(ch_right, min_ch_value, max_ch_value, 255, -255);

    spr.setTextColor(WHITE); 

    spr.setCursor(150, 210);
    spr.printf("RF0: %d\n", ch_left);
    spr.setCursor(150, 230);
    spr.printf("RF1: %d\n", ch_right);
  }  
  else
  {
    gui_text(spr, margin + margin + text_margin_x + widget_width, margin + text_margin_y * 1 + widget_title_height, "ch0 0.00", COLOR_LIGHTGRAY);
    gui_text(spr, margin + margin + text_margin_x + widget_width, margin + text_margin_y * 2 + widget_title_height, "ch1 0.00", COLOR_LIGHTGRAY);
  }

  if (ina219_output_connected && ina219_input_connected) {

    auto shuntvoltage = ina219_output.getShuntVoltage_mV();
    auto busvoltage = ina219_output.getBusVoltage_V();
    auto current_mA = ina219_output.getCurrent_mA();
    auto power_mW = ina219_output.getPower_mW();
    auto loadvoltage = busvoltage + (shuntvoltage / 1000);

    spr.setTextColor(ORANGE); 

    spr.setCursor(150, 110);
    spr.printf("V: %.2fV\n", busvoltage);
    spr.setCursor(150, 130);
    spr.printf("I: %.2fmA\n", current_mA);

    spr.setCursor(150, 160);
    spr.printf("V: %.2fV\n", ina219_input.getBusVoltage_V());
    spr.setCursor(150, 180);
    spr.printf("I: %.2fmA\n", ina219_input.getCurrent_mA());
  }
  else
  {
    gui_text(spr, 
    margin + widget_width + margin + widget_width + margin + text_margin_x, 
    margin + widget_title_height + text_margin_y * 1, 
    "0.00 V", COLOR_LIGHTGRAY);

    gui_text(spr, 
    margin + widget_width + margin + widget_width + margin + text_margin_x, 
    margin + widget_title_height + text_margin_y * 2, 
    "0.00 mA", COLOR_LIGHTGRAY);

    gui_text(spr, 
    margin + widget_width + margin + widget_width + margin + text_margin_x, 
    margin + widget_title_height + text_margin_y * 3, 
    "0.00 V", COLOR_LIGHTGRAY);

    gui_text(spr, 
    margin + widget_width + margin + widget_width + margin + text_margin_x, 
    margin + widget_title_height + text_margin_y * 4, 
    "0.00 mA", COLOR_LIGHTGRAY);
  }

  if (motor_driver_connected) {
    spr.setTextColor(GREEN);

    for (int8_t i = 0; i < 4; i++) {
      spr.setCursor(150, (i + 1) * 20 ); //+ 8 * 15
      spr.printf("Enc%d: %d\n", i, readEncoder(i));
    }

    spr.setTextColor(RED); 
    spr.setCursor(0, (10 * 20));
    spr.printf("Left: %d\n", left_speed);
    spr.setCursor(0, (11 * 20));
    spr.printf("Right: %d\n", right_speed);

    MotorRun(1, -left_speed);
    MotorRun(2, -left_speed);

    MotorRun(3, right_speed);
    MotorRun(0, right_speed);
  }
  else
  {
    gui_text(spr, 
      margin + widget_width + margin + text_margin_x, 
      margin + widget_s_height + margin + widget_m_height + margin + widget_title_height + text_margin_y * 1, 
      "l 0.00%", COLOR_LIGHTGRAY);

    gui_text(spr, 
      margin + widget_width + margin + text_margin_x, 
      margin + widget_s_height + margin + widget_m_height + margin + widget_title_height + text_margin_y * 2, 
      "r 0.00%", COLOR_LIGHTGRAY);


    gui_text(spr, 
      margin + widget_width + margin + text_margin_x, 
      margin + widget_s_height  + margin + widget_title_height + text_margin_y * 1, 
      "ch0 0.00", COLOR_LIGHTGRAY);

    gui_text(spr, 
      margin + widget_width + margin + text_margin_x, 
      margin + widget_s_height + margin + widget_title_height + text_margin_y * 2, 
      "ch1 0.00", COLOR_LIGHTGRAY);

    gui_text(spr, 
      margin + widget_width + margin + text_margin_x, 
      margin + widget_s_height  + margin + widget_title_height + text_margin_y * 3, 
      "ch2 0.00", COLOR_LIGHTGRAY);

    gui_text(spr, 
      margin + widget_width + margin + text_margin_x, 
      margin + widget_s_height + margin + widget_title_height + text_margin_y * 4, 
      "ch3 0.00", COLOR_LIGHTGRAY);
  }

  spr.pushSprite(0, 0);
}