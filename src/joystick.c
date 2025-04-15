#include <joystick.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

float joystick_deadzone = 0;

void init_joystick(float deadzone) {
    joystick_deadzone = deadzone;

    adc_init();
    adc_gpio_init(JOYSTICK_X_PIN);
    adc_gpio_init(JOYSTICK_Y_PIN);
    
    gpio_init(JOYSTICK_BTN);
    gpio_set_dir(JOYSTICK_BTN, GPIO_IN);
    gpio_pull_up(JOYSTICK_BTN);
}

float get_joystick_axis(bool axis) {
    // axis -> 0=x, 1=y
    adc_select_input(axis);
    uint16_t value = adc_read();

    // value is 12-bit 
    float reading = (value - 2048)/2048.0;
    if (reading > joystick_deadzone) return reading; 
    else if (reading < joystick_deadzone) return reading; 
    return 0;
}

edge_t get_joystick_btn() {
    static uint8_t button_pressed = 0;
    return detect_button_edge(JOYSTICK_BTN, &button_pressed);
}
