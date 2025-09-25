#pragma once

#include "freertos/queue.h"

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
  return xQueueReceive(queue.handle, &data, timeout / portTICK_PERIOD_MS) == pdPASS;
}