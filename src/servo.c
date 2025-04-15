#include <servo.h>
#include "hardware/pwm.h"

void init_servo(uint gpio) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(gpio);
    pwm_set_wrap(slice, PWM_WRAP);  // Set frequency once per slice
    pwm_set_clkdiv(slice, 64.0);
    pwm_set_enabled(slice, true);
}

void set_servo_angle(uint gpio, float angle) {
    if (angle > 180 || angle < 0) {
        return;
    }

    float min_pulse = 0.55;  // 550 us (0 degrees)
    float max_pulse = 2.4;  // 2.4 ms (180 degrees)
    float pulse = min_pulse + (angle / 180.0) * (max_pulse - min_pulse);

    uint duty = (pulse * PWM_WRAP) / 20.0;
    pwm_set_chan_level(pwm_gpio_to_slice_num(gpio), pwm_gpio_to_channel(gpio), duty);
}

