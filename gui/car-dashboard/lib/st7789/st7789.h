#pragma once

typedef void (*esp_lcd_flush_ready_cb_t)(void *user_ctx);

void lcd_setBrightness(uint8_t value);
void lcd_setRotation(uint8_t rotation);

void lcd_init(const esp_lcd_flush_ready_cb_t& cb, void* context);
void lcd_display(void* user_data,
                    uint16_t x,
                    uint16_t y,
                    uint16_t x2,
                    uint16_t y2,
                    uint16_t *data);