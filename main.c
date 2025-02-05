#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/gpio.h"
#include "include/led_rgb.h"

#define BUTTON_A 5
#define BUTTON_B 6

volatile bool timer_active = false;
volatile bool debounce_active = false; 

void initialize_leds_and_button()
{
    gpio_init(LED_B);
    gpio_init(LED_R);
    gpio_init(LED_G);
    gpio_init(BUTTON_A);
    gpio_init(BUTTON_B);

    gpio_set_dir(LED_B, GPIO_OUT);
    gpio_set_dir(LED_R, GPIO_OUT);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    gpio_pull_up(BUTTON_B);

    printf("Testando inicialização: \n");

    gpio_put(LED_B, 1);
    printf("LED Azul ligado.\n");
    sleep_ms(500);
    gpio_put(LED_B, 0);
    printf("LED Azul desligado.\n");

    gpio_put(LED_R, 1);
    printf("LED Vermelho ligado.\n");
    sleep_ms(500);
    gpio_put(LED_R, 0);
    printf("LED Vermelho desligado.\n");

    gpio_put(LED_G, 1);
    printf("LED Verde ligado.\n");
    sleep_ms(500);
    gpio_put(LED_G, 0);
    printf("LED Verde desligado.\n");

    printf("Teste de LEDs concluído.\n");

    printf("Pressione um dos botões para testar...\n");
    while (gpio_get(BUTTON_A) == 1 && gpio_get(BUTTON_B) == 1){
    }
    printf("Botão pressionado! Teste bem-sucedido.\n");
}

int64_t turn_off_last_led_callback(alarm_id_t id, void *user_data){
    gpio_put(LED_R, 0);
    timer_active = false;
    printf("Led Verde Apagado.\n");
    return 0;
}

int64_t turn_off_second_led_callback(alarm_id_t id, void *user_data){
    gpio_put(LED_G, 0);
    add_alarm_in_ms(3000, turn_off_last_led_callback, NULL, false);
    printf("Led Vermelho Apagado.\n");
    return 0;
}

int64_t turn_off_first_led_callback(alarm_id_t id, void *user_data){
    gpio_put(LED_B, 0);
    add_alarm_in_ms(3000, turn_off_second_led_callback, NULL, false);
    printf("Led Azul Apagado.\n");
    return 0;
}

int64_t debounce_callback(alarm_id_t id, void *user_data){
    if (gpio_get(BUTTON_A) == 0 || gpio_get(BUTTON_B) == 0){
        if (!timer_active)
        {
            timer_active = true;

            gpio_put(LED_B, 1);
            gpio_put(LED_G, 1);
            gpio_put(LED_R, 1);
            printf("Todos os LEDs acesos.\n");

            add_alarm_in_ms(3000, turn_off_first_led_callback, NULL, false);
        }
    }
    debounce_active = false;
    return 0;
}

void button_callback(uint gpio, uint32_t events)
{
    if ((gpio == BUTTON_A || gpio == BUTTON_B) && !debounce_active)
    {
        debounce_active = true;
        printf("Interrupção do botão detectada, aplicando debounce...\n");
        add_alarm_in_ms(50, debounce_callback, NULL, false);
    }
}

int main()
{
    stdio_init_all();
    initialize_leds_and_button();

    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, button_callback);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, button_callback);

    while (true)
    {
        sleep_ms(1000);
    }
}