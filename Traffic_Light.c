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
volatile bool night_mode = false; // Variável para armazenar o estado do modo noturno

// // Tarefa para controlar os LEDs da matriz
void vTrafficLightMatrixTask()
{
    const TickType_t red_delay = pdMS_TO_TICKS(10000);
    const TickType_t yellow_delay = pdMS_TO_TICKS(4000);
    const TickType_t green_delay = pdMS_TO_TICKS(10000);

    while (true)
    {
        if (!night_mode)
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

        // Se estiver no modo noturno, só o LED amarelo deve piscar levemente a cada 2 segundos
        if (night_mode)
        {
            // Em modo noturno, o LED amarelo pisca levemente
            if (current_state == YELLOW)
            {
                set_buzzer_level(BUZZER_A, WRAP_PWM_BUZZER / 90); // Buzzer baixo para piscar levemente
                vTaskDelay(pdMS_TO_TICKS(1000));                  // Espera 700ms
                set_buzzer_level(BUZZER_A, 0);                    // Desativa o buzzer
                vTaskDelay(pdMS_TO_TICKS(2000));                  // Espera 2 segundos antes de verificar novamente
            }
            continue;
        }

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

        // Delay para evitar ocupar 100% do processador
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

// Tarefa para controlar o display OLED
void vDisplay3Task()
{
    i2c_init(I2C_PORT, 400 * 1000); // Inicializa o I2C na porta I2C_PORT com 400kHz

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);                    // Configura o pino SDA para I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);                    // Configura o pino SCL para I2C
    gpio_pull_up(I2C_SDA);                                        // Habilita o pull-up interno para SDA
    gpio_pull_up(I2C_SCL);                                        // Habilita o pull-up interno para SCL
    ssd1306_t ssd;                                                // Inicializa a estrutura do display
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd);                                         // Configura o display
    ssd1306_send_data(&ssd);                                      // Envia os dados para o display

    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    bool cor = true;
    while (true)
    {
        // ssd1306_fill(&ssd, !cor);                            // Limpa o display
        // ssd1306_rect(&ssd, 3, 3, 122, 60, cor, !cor);        // Desenha um retângulo
        // ssd1306_line(&ssd, 20, 37, 54, 8, cor);             // Desenha uma linha
        // ssd1306_send_data(&ssd);                             // Atualiza o display

        ssd1306_fill(&ssd, false); // Limpa o display
        ssd1306_rect(&ssd, 2, 34, 61, 62, cor, !cor); 

        // Cabeça
        ssd1306_rect(&ssd, 8, 59, 10, 10, cor, !cor); 
        
        // Ombro
        ssd1306_line(&ssd, 58, 20, 70, 20, true);

        // Corpo (linha)
        ssd1306_vline(&ssd, 58, 20, 36, true);
        ssd1306_vline(&ssd, 70, 26, 36, true);


        // Braço esquerdo
        ssd1306_line(&ssd, 58, 26, 50, 32, true);
        ssd1306_line(&ssd, 50, 32, 46, 32, true);
        ssd1306_line(&ssd, 46, 32, 44, 31, true);
        
        ssd1306_line(&ssd, 58, 20, 50, 28, true);
        ssd1306_line(&ssd, 50, 28, 46, 28, true);
        ssd1306_line(&ssd, 46, 28, 44, 30, true);
        
        // Braço direito
        ssd1306_line(&ssd, 70, 26, 80, 34, true);
        ssd1306_line(&ssd, 70, 20, 82, 30, true);
        ssd1306_line(&ssd, 82, 30, 83, 33, true);
        ssd1306_line(&ssd, 81, 34, 83, 33, true);

        // Perna esquerda
        ssd1306_line(&ssd, 58, 36, 44, 58, true);
        ssd1306_line(&ssd, 64, 40, 52, 58, true);
        ssd1306_line(&ssd, 44, 58, 52, 58, true);
        
        // Perna direita
        ssd1306_line(&ssd, 64, 40, 76, 58, true);
        ssd1306_line(&ssd, 70, 36, 84, 58, true);
        ssd1306_line(&ssd, 76, 58, 84, 58, true);

        // Atualiza o display
        ssd1306_send_data(&ssd);

        sleep_ms(735);
    }
}

// Tarefa para verificar o botão e alternar o modo noturno
void vCheckButtonTask()
{
    bool last_state = true; // Considera botão inicialmente não pressionado
    uint32_t last_time_button_A = 0;

    while (1)
    {
        bool current_state = gpio_get(BUTTON_A);

        if (last_state == true && current_state == false)
        {                                       // Detecta borda de descida
            uint32_t now = get_absolute_time(); // tempo atual em ms

            if ((now - last_time_button_A) >= DEBOUNCE_DELAY)
            {
                last_time_button_A = now;
                night_mode = !night_mode; // Alterna modo
            }
        }

        last_state = current_state;
        vTaskDelay(pdMS_TO_TICKS(10)); // só para aliviar CPU
    }
}
// Trecho para modo BOOTSEL com botão B
#include "pico/bootrom.h"
void gpio_irq_handler(uint gpio, uint32_t events)
{
    reset_usb_boot(0, 0);
}

int main()
{
    // Para ser utilizado o modo BOOTSEL com botão B
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    // Fim do trecho para modo BOOTSEL com botão B

    configure_leds_matrix();    // Configura a matriz de LEDs
    configure_buzzer();         // Configura o buzzer
    // configure_button(BUTTON_A); // Configura o botão A
    // configure_button(BUTTON_B); // Configura o botão B

    xTaskCreate(vTrafficLightMatrixTask, "Traffic Light Matrix Task", configMINIMAL_STACK_SIZE,
                NULL, tskIDLE_PRIORITY, NULL);
    // xTaskCreate(vTrafficLightBuzzerTask, "Traffic Light Buzzer Task", configMINIMAL_STACK_SIZE,
    //             NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(vDisplay3Task, "Cont Task Disp3", configMINIMAL_STACK_SIZE,
                NULL, tskIDLE_PRIORITY, NULL);
    // xTaskCreate(vCheckButtonTask, "Check Button Task", configMINIMAL_STACK_SIZE,
    //             NULL, tskIDLE_PRIORITY, NULL);

    vTaskStartScheduler();

    panic_unsupported();
}