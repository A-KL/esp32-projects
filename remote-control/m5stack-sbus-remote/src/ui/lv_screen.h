#pragma once

#include "lv_widgets.h"

extern WidgetPanel<8> esp_now_values;
extern WidgetPanel<8> sbus_values;
extern WidgetPanel<8> nrf42_values;
extern WidgetPanel<8> ps_values;

extern WidgetPanel<4> encoder_values;
extern WidgetPanel<4> motor_values;

extern WidgetPanel<3> power_values;
extern WidgetPanel<3> acc_values;

void lv_ui_change_tab(uint8_t index);