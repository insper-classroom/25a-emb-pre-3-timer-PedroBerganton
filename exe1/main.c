#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>



const int BTN_PIN_R = 28;
const int LED_PIN_R = 4;

volatile int flag_f_r = 0;

volatile int led_ligada = 0;

repeating_timer_t timer;

//Funcao chamada pelo timer; ela pisca o led a cada 500 ms
bool timer_callback(repeating_timer_t *rt){
    led_ligada = 1 - led_ligada;
    gpio_put(LED_PIN_R, led_ligada);
    return true; //timer fica rodando até que o botao seja pressionado dnv
}

void btn_callback(uint gpio, uint32_t events) {
    if (events == 0x4) { // apertou botao
        flag_f_r = 1;
    } else if (events == 0x8) { // rise edge
    }
}



int main() {
    stdio_init_all();
    gpio_init(LED_PIN_R);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);

    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true, &btn_callback);

    int timer_ligado = 0;
    
    while (true) {
        // se botao tiver pressionado
        if (flag_f_r == 1) {
            flag_f_r = 0;
            
            // Se o timer tiver desligado, liga ele e pisca led, se não, desliga tudo
            if (timer_ligado == 0){
                add_repeating_timer_ms(500, timer_callback, NULL, &timer);
                timer_ligado = 1;
            } else{
                cancel_repeating_timer(&timer);
                gpio_put(LED_PIN_R, 0);
                led_ligada = 0;
                timer_ligado = 0;
            }
        }
    }
}
