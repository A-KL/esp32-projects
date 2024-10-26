#pragma once

#include "freertos/queue.h"

QueueHandle_t xQueue;

typedef struct
{
  int16_t values[16];

} Data_t;

void queue_init()
{
    xQueue = xQueueCreate(5, sizeof(Data_t));
}

void queue_send(Data_t &data)
{
  if(xQueueSend(xQueue, &data, 0) != pdPASS){
  }
}

bool queue_receive(Data_t &data)
{
  return xQueueReceive(xQueue, &data, 10) == pdPASS;
}

