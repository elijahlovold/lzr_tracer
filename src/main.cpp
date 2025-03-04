#include "pico/stdlib.h"
#include <stdio.h>
#include <servo.h>
#include <joystick.h>

#define LZR_CTRL_PIN 16

#define SENSE_X 1
#define SENSE_Y 0.5

void setup() {
    stdio_init_all();

    init_joystick(0.1);
    init_servo(SERVO_H);
    init_servo(SERVO_V);

    gpio_init(LZR_CTRL_PIN);
    gpio_set_dir(LZR_CTRL_PIN, GPIO_OUT);
    gpio_put(LZR_CTRL_PIN, 0);
}

int main() {
    setup();

    bool lazer_state = 0; 
    float angle_x = 0;
    float angle_y = 0;
    while (1) {
        if (get_joystick_btn_state() == edge_t::GPIO_IRQ_EDGE_FALL) {
            lazer_state = !lazer_state;
            gpio_put(LZR_CTRL_PIN, lazer_state);
        }

        float delta_x = get_joystick_axis(JOYSTICK_X_AXIS);
        float delta_y = get_joystick_axis(JOYSTICK_Y_AXIS);

        // printf("X: %.3f, Y: %.3f, Lazer: %d\n", delta_x, delta_y, lazer_state);

        angle_x += delta_x*SENSE_X; 
        angle_x = angle_x > 180 ? 180 : angle_x;
        angle_x = angle_x < 0 ? 0 : angle_x;

        angle_y -= delta_y*SENSE_Y; 
        angle_y = angle_y > 180 ? 180 : angle_y;
        angle_y = angle_y < 0 ? 0 : angle_y;

        set_servo_angle(SERVO_H, angle_x);
        set_servo_angle(SERVO_V, angle_y);

        sleep_ms(30);
    }
}

