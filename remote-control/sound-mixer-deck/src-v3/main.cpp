#include <Arduino.h>
#include <Wire.h>
// #include <Adafruit_SSD1306.h>
#include <oled.h>
#include "logo.h"

#include <SerialTransfer.h>

#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   32
#define SCREEN_ADDRESS  0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define UPDATE_RECT_INFO_PACKET_ID 0 
#define UPDATE_RECT_DATA_PACKET_ID 1

struct __attribute__((packed)) update_rect_info_t {
  char display_index;
  uint16_t x;
  uint16_t y;
  uint16_t w;
  uint16_t h;
} rect_info;

// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

OLED display(SDA, SCL);

SerialTransfer connection;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.begin(115200);
  // Serial.println("Display OK");
  connection.begin(Serial);

  display.begin();
  display.drawString(0, 0, "Display OK");

  // display.draw_bitmap_P(0, 0, LOGO_W, LOGO_H, logo);
  // display.draw_bitmap_P(100,8,22,16,bitmap);
  // display.draw_bitmap_P(100, 8, 16, 16, logo_bmp);

  display.display();

  // while (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { 
  //   Serial.println("SSD1306 FAIL");
  //   delay(1000);
  // }
  // Serial.println("SSD1306 OK");

  // display.clear();
  // display.drawRect(0,0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
  // display.setTextSize(1);
  // display.setTextColor(WHITE);
  // display.setCursor(5, 8);
  // display.print("OK");
}

void loop() 
{
  if (!connection.available()) {
    digitalWrite(LED_BUILTIN, LOW);
    return;
  }

  digitalWrite(LED_BUILTIN, HIGH);

  if (connection.currentPacketID() == UPDATE_RECT_INFO_PACKET_ID)
  {
    connection.rxObj(rect_info);
    display.clear();
    return;
  }

  if (connection.currentPacketID() == UPDATE_RECT_DATA_PACKET_ID
      && connection.bytesRead >= sizeof(uint16_t)
      && rect_info.w != 0
      && rect_info.h != 0)
  {
    uint16_t batch_index = 0;
    connection.rxObj(batch_index);

    const uint32_t rect_size = static_cast<uint32_t>(rect_info.w) * rect_info.h;
    const uint8_t payload_size = connection.bytesRead - sizeof(batch_index);
    const uint32_t first_pixel_index = static_cast<uint32_t>(batch_index) * 8;
    if (first_pixel_index >= rect_size)
      return;

    for (uint8_t i = 0; i < payload_size; i++)
    {
      const uint8_t pixel_byte = connection.packet.rxBuff[sizeof(batch_index) + i];
      const uint32_t byte_pixel_index = first_pixel_index + static_cast<uint32_t>(i) * 8;

      for (uint8_t bit = 0; bit < 8; bit++)
      {
        const uint32_t pixel_index = byte_pixel_index + bit;
        if (pixel_index >= rect_size)
          break;

        if ((pixel_byte & (1 << (7 - bit))) == 0)
          continue;

        const uint32_t pixel_x = static_cast<uint32_t>(rect_info.x)
            + pixel_index % rect_info.w;
        const uint32_t pixel_y = static_cast<uint32_t>(rect_info.y)
            + pixel_index / rect_info.w;
        if (pixel_x < SCREEN_WIDTH && pixel_y < SCREEN_HEIGHT)
          display.draw_pixel(pixel_x, pixel_y);
      }
    }

    display.display();
  }
}