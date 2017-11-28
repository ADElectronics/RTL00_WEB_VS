#ifndef _KALMAN_H_
#define _KALMAN_H_

#include "FreeRTOS.h"
#include "device.h"
#include "rtl8195a/rtl_libc.h"
#include "platform_stdlib.h"

void Kalman_Init();
double Kalman_GetAngle(double newAngle, double newRate, double dt);

#endif // _KALMAN_H_
