#include "./include/led_rgb.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"

void init_rgb(void) {
    gpio_init(LED_R);
    gpio_set_dir(LED_R, GPIO_OUT);
    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_init(LED_B);
    gpio_set_dir(LED_B, GPIO_OUT);
}