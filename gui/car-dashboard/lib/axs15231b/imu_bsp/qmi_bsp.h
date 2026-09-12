#ifndef RTC_BSP_H
#define RTC_BSP_H

#include <Arduino.h>
#include "SensorQMI8658.hpp"

typedef struct 
{
  float accx;
  float accy;
  float accz;
  float gyrox;
  float gyroy;
  float gyroz;
}QmiDate_t;


void qmi_init(void);
QmiDate_t i2c_qmi_get(void);
void i2c_qmi_loop_task(void *arg);
#endif