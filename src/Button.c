#include "Button.h"    // Header com definições relacionadas aos botões

// Variáveis de controle para debounce (evitar múltiplos acionamentos)
uint32_t last_time_irq_A = 0;
uint32_t last_time_irq_B = 0;
volatile bool night_mode = false; // Variável para armazenar o estado do modo noturno

// Função de interrupção para os botões
void button_irq_callback(uint gpio, uint32_t events)
{
    uint32_t current_time = get_absolute_time(); // Pega o tempo atual (para debounce)

    if (gpio == BUTTON_A)
    {
        // Verifica debounce
        if ((current_time - last_time_irq_A) < debounce_delay)
            return;
        last_time_irq_A = current_time;
        night_mode = !night_mode; // Alterna o estado do modo noturno

    }
    else if (gpio == BUTTON_B)
    {
        // Verifica debounce e se o alinhamento está travado
        if ((current_time - last_time_irq_B) < debounce_delay)
            return;
        last_time_irq_B = current_time;
        reset_usb_boot(0, 0); // Reinicia o microcontrolador (modo BOOTSEL)
    }
}

// Função para configurar os botões com interrupções
void configure_button(uint8_t button)
{
    gpio_init(button); // Inicializa o GPIO do botão
    gpio_set_dir(button, GPIO_IN); // Configura como entrada
    gpio_pull_up(button); // Habilita pull-up interno
    gpio_set_irq_enabled_with_callback(button, GPIO_IRQ_EDGE_FALL, true, button_irq_callback); // Habilita interrupção por borda de descida
}