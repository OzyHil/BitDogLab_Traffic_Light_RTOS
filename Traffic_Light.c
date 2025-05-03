#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "font.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include <stdio.h>
#include "Led_Matrix.h" // Inclusão da biblioteca para controlar a matriz de LEDs>
#include "Buzzer.h"     // Inclusão da biblioteca para controlar o buzzer
#include "Button.h"     // Inclusão da biblioteca para controlar os botões

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

#define led1 11
#define led2 12

// Variável global para o estado do semáforo
volatile semaphore_state g_current_state = RED;

// // Tarefa para controlar os LEDs da matriz
void vTrafficLightMatrixTask()
{
    const TickType_t red_delay = pdMS_TO_TICKS(10000);
    const TickType_t yellow_delay = pdMS_TO_TICKS(4000);
    const TickType_t green_delay = pdMS_TO_TICKS(10000);

    while (true)
    {
        if (night_mode)
        {
            draw_traffic_light(RED); // Acende a cor vermelha do semáforo
            g_current_state = RED;
            vTaskDelay(red_delay);

            draw_traffic_light(YELLOW); // Acende a cor amarela do semáforo
            g_current_state = YELLOW;
            vTaskDelay(yellow_delay);

            draw_traffic_light(GREEN); // Acende a cor verde do semáforo
            g_current_state = GREEN;
            vTaskDelay(green_delay);
        }
        else
        {
            draw_traffic_light(YELLOW); // Acende a cor amarela do semáforo
            g_current_state = YELLOW;
            vTaskDelay(yellow_delay);
        }
    }
}

// Tarefa para controlar o buzzer
void vTrafficLightBuzzerTask()
{
    semaphore_state current_state;

    while (true)
    {
        // Lê o estado atual da variável global
        current_state = g_current_state;

        // Processa o estado e gera o som apropriado
        switch (current_state)
        {
        case RED:
            // Tom contínuo curto (500ms ligado e 1.5s desligado) “pare”
            set_buzzer_level(BUZZER_A, WRAP_PWM_BUZZER / 90); // Ativa o buzzer
            vTaskDelay(pdMS_TO_TICKS(500));
            set_buzzer_level(BUZZER_A, 0); // Desativa o buzzer
            vTaskDelay(pdMS_TO_TICKS(1500));
            break;
        case YELLOW:
            // Beep rápido intermitente “atenção”
            for (int i = 0; i < 5; i++)
            {                                                     // 5 beeps
                set_buzzer_level(BUZZER_A, WRAP_PWM_BUZZER / 90); // Ativa o buzzer
                vTaskDelay(pdMS_TO_TICKS(100));                   // Beep curto
                set_buzzer_level(BUZZER_A, 0);                    // Desativa o buzzer
                vTaskDelay(pdMS_TO_TICKS(100));                   // Pausa curta
            }
            break;
        case GREEN:
            // 1 beep curto por um segundo “pode atravessar”
            set_buzzer_level(BUZZER_A, WRAP_PWM_BUZZER / 90); // Ativa o buzzer
            vTaskDelay(pdMS_TO_TICKS(100));                   // Beep curto
            set_buzzer_level(BUZZER_A, 0);                    // Desativa o buzzer
            vTaskDelay(pdMS_TO_TICKS(900));                   // Pausa para completar 1 segundo
            break;
        default:
            // Erro: estado inválido
            set_buzzer_level(BUZZER_A, 0);
            break;
        }

        // Delay para evitar ocupar 100% do processador (ajuste conforme necessário)
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void vDisplay3Task()
{
    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400 * 1000);

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);                    // Set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA);                                        // Pull up the data line
    gpio_pull_up(I2C_SCL);                                        // Pull up the clock line
    ssd1306_t ssd;                                                // Inicializa a estrutura do display
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd);                                         // Configura o display
    ssd1306_send_data(&ssd);                                      // Envia os dados para o display
    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    char str_y[5]; // Buffer para armazenar a string
    int contador = 0;
    bool cor = true;
    while (true)
    {
        sprintf(str_y, "%d", contador);                      // Converte em string
        contador++;                                          // Incrementa o contador
        ssd1306_fill(&ssd, !cor);                            // Limpa o display
        ssd1306_rect(&ssd, 3, 3, 122, 60, cor, !cor);        // Desenha um retângulo
        ssd1306_line(&ssd, 3, 25, 123, 25, cor);             // Desenha uma linha
        ssd1306_line(&ssd, 3, 37, 123, 37, cor);             // Desenha uma linha
        ssd1306_draw_string(&ssd, "CEPEDI   TIC37", 8, 6);   // Desenha uma string
        ssd1306_draw_string(&ssd, "EMBARCATECH", 20, 16);    // Desenha uma string
        ssd1306_draw_string(&ssd, "  FreeRTOS", 10, 28);     // Desenha uma string
        ssd1306_draw_string(&ssd, "Contador  LEDs", 10, 41); // Desenha uma string
        ssd1306_draw_string(&ssd, str_y, 40, 52);            // Desenha uma string
        ssd1306_send_data(&ssd);                             // Atualiza o display
        sleep_ms(735);
    }
}

int main()
{
    configure_leds_matrix();    // Configura a matriz de LEDs
    configure_buzzer();         // Configura o buzzer
    configure_button(BUTTON_A); // Configura o botão A
    configure_button(BUTTON_B); // Configura o botão B

    xTaskCreate(vTrafficLightMatrixTask, "Traffic Light Task", configMINIMAL_STACK_SIZE,
                NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(vTrafficLightBuzzerTask, "Traffic Light Task", configMINIMAL_STACK_SIZE,
                NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(vDisplay3Task, "Cont Task Disp3", configMINIMAL_STACK_SIZE,
                NULL, tskIDLE_PRIORITY, NULL);

    vTaskStartScheduler();

    panic_unsupported();
}