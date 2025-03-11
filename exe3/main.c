#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int LED_PIN_R = 4;

volatile int flag_f_r = 0;
volatile int flag_botao_solto = 0;


void btn_callback(uint gpio, uint32_t events) {
    if (events == 0x4) { // fall edge
        if (gpio == BTN_PIN_R)
            flag_f_r = 1;

    } else if (events == 0x8) { // rise edge
        if (gpio == BTN_PIN_R)
            flag_botao_solto = 1;


    }
}

int main() {
    stdio_init_all();

    gpio_init(LED_PIN_R);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);

    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    gpio_set_irq_enabled_with_callback(
        BTN_PIN_R, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &btn_callback);
    
    uint32_t tempo_pressionado;
    int led_aceso = 0;
    while (true) {

        if (flag_f_r == 1) {
            //pega inicializacao do clique e reseta flag
            tempo_pressionado = to_ms_since_boot(get_absolute_time());
            flag_f_r = 0;
        }

        if (flag_botao_solto == 1){
            //pega o tempo final, ao soltar o botaoo
            uint32_t tempo_solto =  to_ms_since_boot(get_absolute_time());
            if(tempo_solto - tempo_pressionado >=500){
                led_aceso = 1 - led_aceso;
                gpio_put(LED_PIN_R, led_aceso);
            }

            //reseta e volta ao inicio
            flag_botao_solto = 0;
        }
    }
}
