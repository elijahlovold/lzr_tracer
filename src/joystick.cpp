#include <cmath>
#include <joystick.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

float joystick_deadzone = 0;

void init_joystick(float deadzone) {
    joystick_deadzone = deadzone;

    adc_init();
    adc_gpio_init(JOYSTICK_X_PIN);
    adc_gpio_init(JOYSTICK_Y_PIN);
    
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);
}

float get_joystick_axis(bool axis) {
    // axis -> 0=x, 1=y
    adc_select_input(axis);
    uint16_t value = adc_read();

    // value is 12-bit 
    float reading = (value - 2048)/2048.0;
    return fabs(reading) > joystick_deadzone ? reading : 0; 
}

edge_t get_joystick_btn() {
    static bool button_pressed = 0;

    bool button_state = gpio_get(BUTTON_PIN);

    if (!button_pressed && button_state) {
        button_pressed = true;
        return edge_t::GPIO_IRQ_EDGE_RISE;
    } 
    if (button_pressed && !button_state) {
        button_pressed = false;
        return edge_t::GPIO_IRQ_EDGE_FALL;
    }
    if (button_state) {
        return edge_t::GPIO_IRQ_LEVEL_HIGH;
    }
    return edge_t::GPIO_IRQ_LEVEL_LOW;
}
