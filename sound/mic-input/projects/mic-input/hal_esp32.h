#pragma once

void IRAM_ATTR OnLeftButtonPressed()
{
    log_w("Left button Pressed!");
  //digitalWrite(LED_pin, !digitalRead(LED_pin));
}

void IRAM_ATTR OnRightButtonPressed()
{
    log_w("Right button Pressed!");
  //digitalWrite(LED_pin, !digitalRead(LED_pin));
}

void gui_update_task(void *arg)  
{
    while (1) 
    {
        gui_progress_bars_update();
        vTaskDelay(100 / portTICK_RATE_MS);
    }
}

void hal_gui_start() {
  xTaskCreate(gui_update_task, "gui_task", 2048, NULL, 1, NULL);
}

void hal_init() {
    Serial.begin(115200);
    delay(1000);
}

void hal_buttons_init() {
    pinMode(0, INPUT_PULLUP);
    pinMode(35, INPUT_PULLUP);
    attachInterrupt(0, OnLeftButtonPressed, RISING);
    attachInterrupt(35, OnRightButtonPressed, RISING);
}