#include <utils.h>

edge_t detect_button_edge(uint8_t gpio_pin, uint8_t *previous_state) {
    uint8_t current_state = gpio_get(gpio_pin);
    if (!*previous_state && current_state) {
        *previous_state = true;
        return GPIO_IRQ_EDGE_RISE;
    }
    if (*previous_state && !current_state) {
        *previous_state = false;
        return GPIO_IRQ_EDGE_FALL;
    }
    return current_state ? GPIO_IRQ_LEVEL_HIGH : GPIO_IRQ_LEVEL_LOW;
}
