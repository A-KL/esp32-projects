#pragma once

#include <lvgl.h>
#include <TFT_eSPI.h>

#define LVGL_LCD_BUF_SIZE ((TFT_WIDTH*TFT_HEIGHT)/10)

static TFT_eSPI tft = TFT_eSPI();

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[LVGL_LCD_BUF_SIZE];

static TaskHandle_t lv_tick_task = NULL;

/* LVGL calls it when a rendered image needs to copied to the display */
void hal_display_flush_cb(lv_disp_drv_t* disp_drv, const lv_area_t* area, lv_color_t* color_p)
{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

    tft.startWrite();
    tft.setAddrWindow( area->x1, area->y1, w, h );
    tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
    tft.endWrite();

    lv_disp_flush_ready( disp_drv );
    // auto size = lv_area_get_size(area);

    // /* Set TFT address window to clipped image bounds */

    // // uint32_t w = (area->x2 - area->x1 + 1);
    // // uint32_t h = (area->y2 - area->y1 + 1);
    // // lcd_PushColors(area->x1, area->y1, w, h, (uint16_t *)&color_p->full);

    // tft.setAddrWindow(area->x1, area->y1, area->x2, area->y2);
    // tft.pushColors((uint16_t*)&color_p->full, size, true );
    // tft.setAddrWindow(0, 0, tft.width() - 1, tft.height() - 1); //restore full screen

    // /* Call it to tell LVGL you are ready */
    // lv_disp_flush_ready(disp);
}

static void hal_timer_tick(void * pvParameters)
{
    while(1) {
        vTaskDelay(5);
        lv_tick_inc(5);
    }
}

#if LV_USE_LOG != 0

void hal_log_cb(const char * buf)
{
    log_i("LOG: %s\r\n", buf);
      /*Send the logs via serial port*/
  // if(level == LV_LOG_LEVEL_ERROR) serial_send("ERROR: ");
  // if(level == LV_LOG_LEVEL_WARN)  serial_send("WARNING: ");
  // if(level == LV_LOG_LEVEL_INFO)  serial_send("INFO: ");
  // if(level == LV_LOG_LEVEL_TRACE) serial_send("TRACE: ");
}

#endif

void lcd_init()
{
  tft.init();
  tft.setRotation(TFT_ROTATE);
  tft.fillScreen(TFT_RED); //TFT_BLACK
}

void hal_init()
{
#if LV_USE_LOG != 0
    lv_log_register_print_cb(hal_log_cb); 
#endif

  lv_disp_draw_buf_init(&draw_buf, buf, NULL, LVGL_LCD_BUF_SIZE);

  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);

  disp_drv.hor_res = TFT_HEIGHT;
  disp_drv.ver_res =  TFT_WIDTH;
  disp_drv.flush_cb = hal_display_flush_cb;
  disp_drv.draw_buf = &draw_buf;

  lv_disp_drv_register(&disp_drv);

    /*Initialize the (dummy) input device driver*/
    // stc lv_indev_drv_t indev_drv;
    // lv_indev_drv_init( &indev_drv );
    // indev_drv.type = LV_INDEV_TYPE_POINTER;
    // indev_drv.read_cb = my_touchpad_read;
    // lv_indev_drv_register( &indev_drv );ati

  xTaskCreate(hal_timer_tick, "lv_tick_thread", 2048, NULL, tskIDLE_PRIORITY, &lv_tick_task);
}

void hal_loop(void)
{
  delay(2);
  lv_timer_handler();
}