#ifndef LED_MATRIX_H
#define LED_MATRIX_H

#include <stdlib.h>
#include "pico/stdlib.h"
#include "pio_matrix.pio.h" // Programa específico para controle da matriz de LEDs

#define NUM_PIXELS 25 // Total de LEDs na matriz (5x5)
#define LED_MATRIX 7 // GPIO para controle da matriz de LEDs

// Estrutura para armazenar referências do PIO
typedef struct
{
    PIO ref;              // Referência ao PIO (pio0 ou pio1)
    uint offset;          // Offset do programa carregado
    uint state_machine;   // Máquina de estado usada
} refs;

// Estrutura para representar uma cor RGB
typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} led_color;

// Estados do semáforo
typedef enum {
    RED = 4,    // Cor vermelha
    YELLOW = 5, // Cor amarela
    GREEN = 6   // Cor verde
} semaphore_state;

void configure_leds_matrix(); // Função para configurar a matriz de LEDs

// Converte uma estrutura de cor RGB para um valor 32 bits
uint32_t rgb_matrix(led_color color);

// Função para desenhar as cores do semáforo
void draw_traffic_light(uint8_t color_code);

#endif
