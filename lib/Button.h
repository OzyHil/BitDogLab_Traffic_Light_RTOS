#ifndef BUTTON_H
#define BUTTON_H

// Inclusão da biblioteca geral do sistema
#include "General.h"

// Definição dos pinos dos botões
#define BUTTON_A 5 // Botão A   
#define BUTTON_B 6  // Botão B
#define debounce_delay 300000 // Tempo de debounce em microssegundos

extern volatile bool night_mode; // Variável para armazenar o estado do modo noturno

// Função de callback para interrupção de botões
void button_irq_callback(uint gpio, uint32_t events);

// Função para configurar os botões
void configure_button(uint8_t button);

#endif