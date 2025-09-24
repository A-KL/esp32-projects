#pragma once

extern WidgetPanel<8> esp_now_values;
extern WidgetPanel<8> sbus_values;
extern WidgetPanel<8> nrf42_values;
extern WidgetPanel<8> ps_values;

extern WidgetPanel<4> encoder_values;
extern WidgetPanel<4> motors_values;

extern WidgetPanel<6> power_values;

void lv_ui_change_tab(uint8_t index);