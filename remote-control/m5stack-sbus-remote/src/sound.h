#pragma once

#include "Speaker.h"

static SPEAKER Speaker;
static volatile bool _state = false;
TaskHandle_t sound_task_handle = NULL;

void sound_task(void * parameter)
{
    while (_state)
    {
        Speaker.tone(1000);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        Speaker.end();
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    
    vTaskDelete(NULL);
}

void sound_init() {
  Speaker.begin();
}

void sound_on(bool state)
{
    _state = state;

    if (state && sound_task_handle == NULL) {
        xTaskCreate(sound_task, "TASK1", 512, NULL, 1, &sound_task_handle);
    }

    // if (!state && sound_task_handle != NULL) {
    //     xTaskCreate(sound_task, "TASK1", 512, NULL, 1, &sound_task_handle);
    // }
}