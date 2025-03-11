#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int BTN_PIN_G = 26;

const int LED_PIN_R = 4;
const int LED_PIN_G = 6;

volatile int flag_r = 0;
volatile int flag_g = 0;

repeating_timer_t timer_RED;
repeating_timer_t timer_GREEN;

//Funcao chamada pelo timer; ela pisca o led a cada 500 ms
bool timer_callback_RED(repeating_timer_t *rt){
    led_ligada_R = 1 - led_ligada_R;
    gpio_put(LED_PIN_R, led_ligada_R);
    return true; //timer fica rodando até que o botao seja pressionado dnv
}

//Funcao chamada pelo timer; ela pisca o led a cada 250 ms
bool timer_callback_GREEN(repeating_timer_t *rt){
    led_ligada_G = 1 - led_ligada_G;
    gpio_put(LED_PIN_G, led_ligada_G);
    return true; //timer fica rodando até que o botao seja pressionado dnv
}

void btn_callback(uint gpio, uint32_t events) {
    if (events == 0x4) {
        if (gpio == BTN_PIN_R)
            flag_r = 1;
        else if (gpio == BTN_PIN_G)
            flag_g = 1;
    }
}

volatile int timer_ligado_R = 0;
volatile int timer_ligado_G = 0;

int main() {
    gpio_init(LED_PIN_R);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);

    gpio_init(LED_PIN_G);
    gpio_set_dir(LED_PIN_G, GPIO_OUT);

    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    gpio_init(BTN_PIN_G);
    gpio_set_dir(BTN_PIN_G, GPIO_IN);
    gpio_pull_up(BTN_PIN_G);

    gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true, &btn_callback);
    gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_FALL, true);

    while (true) {

        if (flag_r == 1)  {
            flag_r = 0;
            
            // Se o timer tiver desligado, liga ele e pisca led, se não, desliga tudo

            if (timer_ligado_R == 0){
                add_repeating_timer_ms(500, timer_callback_RED, NULL, &timer_RED);
                timer_ligado_R = 1;
            } else{
                cancel_repeating_timer(&timer_RED);
                gpio_put(LED_PIN_R, 0);
                led_ligada_R = 0;
                timer_ligado_R = 0;


            }
        }

        if (flag_g == 1) {
            flag_g = 0;
            
            // Se o timer tiver desligado, liga ele e pisca led, se não, desliga tudo
            if (timer_ligado_G == 0){
                add_repeating_timer_ms(250, timer_callback_GREEN, NULL, &timer_GREEN);
                timer_ligado_G = 1;
            } else{
                cancel_repeating_timer(&timer_GREEN);
                gpio_put(LED_PIN_G, 0);
                led_ligada_G = 0;
                timer_ligado_G = 0;


            }
        }
    }
}
