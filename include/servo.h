#ifndef _H_SERVO
#define _H_SERVO 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "pico/stdlib.h"

#define SERVO_X 15  // GPIO connected to servo signal wire
#define SERVO_Y 14  // GPIO connected to servo signal wire

#define PWM_WRAP 39062 // 50 Hz PWM period
                       
void init_servo(uint gpio);
void set_servo_angle(uint gpio, float angle);

#ifdef __cplusplus
}
#endif

#endif
