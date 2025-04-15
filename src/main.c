#include <utils.h>
#include <stdio.h>
#include <servo.h>
#include <sd_rec.h>
#include <joystick.h>

#define BTN_REC 2
#define BTN_STM 3

#define LZR_CTRL_PIN 16

#define SENSE_X 1.5
#define SENSE_Y 1.0

#define SAMPLE_RATE 25
#define PERIOD_MS (int)(1e3/SAMPLE_RATE)

int sd_enabled = 0;

void set_lzr_state(l_state laser_on);
void apply_delta(float *theta, float delta, uint servo);
int play_recording();
edge_t get_rec_btn();
edge_t get_stream_btn();

void setup() {
    stdio_init_all();

    gpio_init(BTN_REC); gpio_set_dir(BTN_REC, GPIO_IN); gpio_pull_up(BTN_REC);
    gpio_init(BTN_STM); gpio_set_dir(BTN_STM, GPIO_IN); gpio_pull_up(BTN_STM);

    init_joystick(0.1);
    init_servo(SERVO_X);
    init_servo(SERVO_Y);
    sd_enabled = init_sd();

    gpio_init(LZR_CTRL_PIN);
    gpio_set_dir(LZR_CTRL_PIN, GPIO_OUT);
    gpio_put(LZR_CTRL_PIN, 0);
}

int main() {
    setup();
    struct lzr_state state = {0, 0, 0};

    // while (!stdio_usb_connected()) sleep_ms(100);

    while (1) {
        if (get_stream_btn() == GPIO_IRQ_EDGE_FALL) {
            play_recording();
            continue;
        }

        if (get_rec_btn() == GPIO_IRQ_EDGE_FALL) {
            if (!recording_active()) start_recording(SAMPLE_RATE, 0);
            else end_recording();
        }

        if (get_joystick_btn() == GPIO_IRQ_EDGE_FALL) {
            state.laser_on = !state.laser_on;
            set_lzr_state(state.laser_on);
        }

        float delta_x = get_joystick_axis(JOYSTICK_X_AXIS);
        float delta_y = get_joystick_axis(JOYSTICK_Y_AXIS);

        apply_delta(&state.th_x, delta_x, SERVO_X);
        apply_delta(&state.th_y, delta_y, SERVO_Y);

        if (recording_active()) {
            printf("Rec - X: %.3f, Y: %.3f, Lazer: %d\n", state.th_x, state.th_y, state.laser_on);
            record_pt(&state, 1);
        }

        sleep_ms(PERIOD_MS);
    }
}

void set_lzr_state(l_state laser_on) {
    gpio_put(LZR_CTRL_PIN, laser_on);
}

void apply_delta(float *theta, float delta, uint servo) {
    *theta -= delta*((servo == SERVO_X) ? SENSE_X : SENSE_Y); 
    *theta = *theta > 180 ? 180 : *theta;
    *theta = *theta < 0 ? 0 : *theta;
    set_servo_angle(servo, *theta);
}

int play_recording() {
    unsigned int sample_rate;
    if (!start_streaming(&sample_rate, 0) || sample_rate == 0) return 0;

    unsigned int delay_us = 1e6 / sample_rate;

    l_state laser_on = 0;
    struct lzr_state state;
    while(stream_pt(&state)) {
        if (laser_on != state.laser_on) {
            laser_on = state.laser_on;
            set_lzr_state(laser_on);
        }
        set_servo_angle(SERVO_X, state.th_x);
        set_servo_angle(SERVO_Y, state.th_y);

        if (get_joystick_btn() == GPIO_IRQ_EDGE_FALL) break;
        if (get_stream_btn() == GPIO_IRQ_EDGE_FALL) break;

        printf("Stream - X: %.3f, Y: %.3f, Lazer: %d\n", state.th_x, state.th_y, state.laser_on);
        sleep_us(delay_us);
    }
    end_streaming();
}

edge_t get_rec_btn() {
    static uint8_t button_pressed = 0;
    return detect_button_edge(BTN_REC, &button_pressed);
}

edge_t get_stream_btn() {
    static uint8_t button_pressed = 0;
    return detect_button_edge(BTN_STM, &button_pressed);
}
