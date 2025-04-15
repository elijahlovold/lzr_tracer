#ifndef _H_JOYSTICK 
#define _H_JOYSTICK

#ifdef __cplusplus
extern "C" {
#endif

#include <utils.h>

#define JOYSTICK_X_PIN  26  // GPIO26 -> ADC0
#define JOYSTICK_Y_PIN  27  // GPIO27 -> ADC1
#define JOYSTICK_BTN    22  // Digital button input

#define JOYSTICK_X_AXIS 0
#define JOYSTICK_Y_AXIS 1

void init_joystick(float deadzone);
float get_joystick_axis(bool axis);
edge_t get_joystick_btn();

#ifdef __cplusplus
}
#endif

#endif
