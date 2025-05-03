#include "Led_Matrix.h" // Inclusão da biblioteca para controlar a matriz de LEDs


led_color DIGIT_COLORS[7]; // Cores dos dígitos

refs pio;

// Matriz que será usada como base para colorir
const int8_t matrix[NUM_PIXELS] = {
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    3,
    0,
    0,
    0,
    0,
    2,
    0,
    0,
    0,
    0,
    1,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
};

void init_digit_colors() {
  DIGIT_COLORS[0] = DARK;
  DIGIT_COLORS[1] = RED_DIM;
  DIGIT_COLORS[2] = YELLOW_DIM;
  DIGIT_COLORS[3] = GREEN_DIM;
  DIGIT_COLORS[4] = RED;
  DIGIT_COLORS[5] = YELLOW;
  DIGIT_COLORS[6] = GREEN;
}

void configure_leds_matrix()
{
  pio.ref = pio0;

  pio.state_machine = pio_claim_unused_sm(pio.ref, true);     // Obtém uma máquina de estado livre
  pio.offset = pio_add_program(pio.ref, &pio_matrix_program); // Adiciona o programa da matriz

  pio_matrix_program_init(pio.ref, pio.state_machine, pio.offset, LED_MATRIX); // Inicializa o programa

  init_digit_colors(); // Inicializa as cores dos dígitos
}

// Converte uma estrutura de cor RGB para um valor 32 bits conforme o protocolo da matriz
uint32_t rgb_matrix(led_color color)
{
  return (color.green << 24) | (color.red << 16) | (color.blue << 8);
}

// Função para desenhar as cores do semáforo
void draw_traffic_light(uint8_t color_code)
{
  for (int8_t i = 0; i < NUM_PIXELS; i++)
  {
    uint8_t led_index = matrix[i];
    uint32_t color_to_send;

    if (led_index == 0)
    {                                              // Apaga onde tiver 0 na matriz
      color_to_send = rgb_matrix(DIGIT_COLORS[0]); // Preto
    }
    else if (led_index == (color_code - 3))
    {                                                       // Checa se a cor deve acender
      color_to_send = rgb_matrix(DIGIT_COLORS[color_code]); // Cor acesa
    }
    else
    {
      color_to_send = rgb_matrix(DIGIT_COLORS[led_index]); // Cor apagada
    }

    pio_sm_put_blocking(pio.ref, pio.state_machine, color_to_send);
  }
}