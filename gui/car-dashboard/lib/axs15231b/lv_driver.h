#pragma once

#include <Arduino.h>
#include <lvgl.h>

#include <Arduino_GFX_Library.h>

Arduino_DataBus *bus = new Arduino_ESP32QSPI(TFT_CS, TFT_SCK, TFT_SDA0, TFT_SDA1, TFT_SDA2, TFT_SDA3);
Arduino_GFX *g = new Arduino_AXS15231B(bus, TFT_RST, 0, false, TFT_WIDTH, TFT_HEIGHT);
Arduino_Canvas *gfx = new Arduino_Canvas(TFT_WIDTH, TFT_HEIGHT, g, 0, 0, 1);

// void hal_display_flush_ready_cb(void *user_ctx)
// {
//   auto disp_driver = (lv_disp_drv_t *)user_ctx;
//   lv_disp_flush_ready(disp_driver);
// }

void lv_lcd_flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = lv_area_get_width(area);
  uint32_t h = lv_area_get_height(area);

 // gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);

  lv_disp_flush_ready(drv);
}
void lv_lcd_loop()
{
  gfx->flush();
}

void lv_lcd_init()
{
  // Display setup
  if(!gfx->begin(40000000UL)) {
      Serial.println("Failed to initialize display!");
      return;
  }
  gfx->fillScreen(0xFFFF);

  // Switch backlight on
  // pinMode(TFT_BL, OUTPUT);
  // digitalWrite(TFT_BL, HIGH);

  static lv_disp_drv_t disp_drv;
  static lv_disp_draw_buf_t disp_buf;
  static lv_color_t *lv_disp_buf;

  // lv_disp_buf = (lv_color_t *)ps_malloc(sizeof(lv_color_t) * LVGL_LCD_BUF_SIZE);
  // assert(lv_disp_buf);

  // lv_disp_draw_buf_init(&disp_buf, lv_disp_buf, NULL, LVGL_LCD_BUF_SIZE);

  // /*Initialize the display*/
  // ESP_LOGI(TAG, "Register display driver to LVGL");
  // lv_disp_drv_init(&disp_drv);
  // disp_drv.hor_res = EXAMPLE_LCD_H_RES;
  // disp_drv.ver_res = EXAMPLE_LCD_V_RES;
  // disp_drv.flush_cb = lv_lcd_flush_cb;
  // disp_drv.draw_buf = &disp_buf;
  // // disp_drv.full_refresh = 1;          //full_refresh must be 1
  // // disp_drv.user_data = &panel_handle;
  // lv_disp_drv_register(&disp_drv);
  
  lv_disp_buf = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * LVGL_LCD_BUF_SIZE, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  if (!lv_disp_buf) {
      Serial.println("LVGL failed to allocate display buffer!");
      return;
  }
  lv_disp_draw_buf_init(&disp_buf, lv_disp_buf, NULL, LVGL_LCD_BUF_SIZE);

  /*Initialize the display*/
  ESP_LOGI(TAG, "Register display driver to LVGL");
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = EXAMPLE_LCD_H_RES;
  disp_drv.ver_res = EXAMPLE_LCD_V_RES;
  disp_drv.flush_cb = lv_lcd_flush_cb;
  disp_drv.draw_buf = &disp_buf;
  // disp_drv.full_refresh = 1;          //full_refresh must be 1
  // disp_drv.user_data = &panel_handle;
  lv_disp_drv_register(&disp_drv);
}