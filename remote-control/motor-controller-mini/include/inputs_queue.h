#pragma once

#include "freertos/queue.h"

// V1

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

// V2
template<typename TData>
struct queue_t 
{
  QueueHandle_t handle;
  size_t data_size = sizeof(TData);
};

template<typename TData>
void queue_init(queue_t<TData>& queue, uint8_t queue_size = 5)
{
    queue.handle = xQueueCreate(queue_size, queue.data_size);
}

template<typename TData>
bool queue_send(const queue_t<TData>& queue, const TData& data, const uint16_t timeout = 0)
{
  return xQueueSend(queue.handle, &data, timeout) == pdPASS;
}

template<typename TData>
bool queue_receive(const queue_t<TData>& queue, TData& data, const uint16_t timeout = 10)
{
  return xQueueReceive(queue.handle, &data, timeout) == pdPASS;
}
