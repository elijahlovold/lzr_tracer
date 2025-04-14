#ifndef _H_JOYSTICK 
#define _H_JOYSTICK

#include <stdio.h>
#include "hardware/gpio.h"

#define JOYSTICK_X_PIN  26  // GPIO26 -> ADC0
#define JOYSTICK_Y_PIN  27  // GPIO27 -> ADC1
#define BUTTON_PIN      22  // Digital button input

#define JOYSTICK_X_AXIS 0
#define JOYSTICK_Y_AXIS 1

using edge_t = gpio_irq_level;

void init_joystick(float deadzone);
float get_joystick_axis(bool axis);
edge_t get_joystick_btn();

#endif
