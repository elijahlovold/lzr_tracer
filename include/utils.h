#ifndef _H_UTILS
#define _H_UTILS 1

#ifdef __cplusplus
extern "C" {
#endif

#include "pico/stdlib.h"
#include <hardware/gpio.h>

typedef enum gpio_irq_level edge_t;
typedef unsigned char l_state;

struct lzr_state {
    float th_x;
    float th_y;
    l_state laser_on;
};

edge_t detect_button_edge(uint8_t gpio_pin, uint8_t *previous_state);


#ifdef __cplusplus
}
#endif

#endif
