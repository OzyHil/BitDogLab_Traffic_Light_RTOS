#include "General.h"    // Inclusão da biblioteca geral com definições e funções comuns
#include "Led_Matrix.h" // Inclusão da biblioteca para controlar a matriz de LEDs>
#include "Buzzer.h"     // Inclusão da biblioteca para controlar o buzzer
#include "Button.h"     // Inclusão da biblioteca para controlar os botões
#include "Display.h"    // Inclusão da biblioteca para controlar o display OLED
#include "Led.h"        // Inclusão da biblioteca para controlar os LEDs

// Variável global para o estado do semáforo
volatile semaphore_state g_current_state = SEMAPHORE_RED; // Inicializa o estado do semáforo de veículos como vermelho
volatile bool night_mode = false;                         // Variável para armazenar o estado do modo noturno

// // Tarefa para controlar os LEDs da matriz
void vTrafficLightMatrixTask()
{
    const TickType_t check_interval = pdMS_TO_TICKS(500); // 500ms de intervalo para verificar o estado do semáforo
    const int red_ticks = 20;                             // Número de ticks para o vermelho
    const int yellow_ticks = 8;                           // Número de ticks para o amarelo
    const int green_ticks = 20;                           // Número de ticks para o verde

    while (true)
    {
        // Verificas se o modo noturno não está ativado
        if (!night_mode)
        {
            draw_traffic_light(SEMAPHORE_RED); // Desenha o semáforo vermelho
            g_current_state = SEMAPHORE_RED;   // Atualiza o estado atual do semafóro de veiculos

            // Aguarda o tempo total do sinal vermelho (10s), mas verifica a cada 100 ms se o modo noturno foi ativado para interromper imediatamente, se necessário
            for (int i = 0; i < red_ticks && !night_mode; i++)
                vTaskDelay(check_interval); // Espera 1s

            draw_traffic_light(SEMAPHORE_YELLOW);
            g_current_state = SEMAPHORE_YELLOW;
            for (int i = 0; i < yellow_ticks && !night_mode; i++)
                vTaskDelay(check_interval);

            draw_traffic_light(SEMAPHORE_GREEN);
            g_current_state = SEMAPHORE_GREEN;
            for (int i = 0; i < green_ticks && !night_mode; i++)
                vTaskDelay(check_interval);
        }
        else if (night_mode)
        {
            draw_traffic_light(SEMAPHORE_YELLOW);
            g_current_state = SEMAPHORE_YELLOW;
            vTaskDelay(pdMS_TO_TICKS(100));
        }
    }
}

// Tarefa para controlar o buzzer
void vTrafficLightBuzzerTask()
{
    while (true)
    {
        if (night_mode)
        {

            if (g_current_state == SEMAPHORE_YELLOW) // Verifica se o semafóro está amarelo
            {
                beep_mode_night(); // Buzzer em modo noturno. 1s ligado e 2s desligado
            }
            continue;
        }

        // Processa o estado do semáforo e gera o som apropriado
        switch (g_current_state)
        {
        case SEMAPHORE_RED:
            beep_mode_red(); // Beep longo intermitente “pare”. 0,5s ligado e 1,5s desligado
            break;
        case SEMAPHORE_YELLOW:
            beep_mode_yellow(); // Beep rápido intermitente “atenção”. 0,1s ligado e 0,1s desligado
            break;
        case SEMAPHORE_GREEN:
            beep_mode_green(); // 1 beep curto por um segundo. “pode atravessar”
            break;
        default:
            set_buzzer_level(BUZZER_A, 0); // Erro: estado inválido
            break;
        }

        vTaskDelay(pdMS_TO_TICKS(10)); // Delay para evitar ocupar 100% do processador
    }
}

// Tarefa para controlar o display OLED
void vPedestrianDisplayTask()
{
    while (true)
    {
        if (!night_mode)
        {
            switch (g_current_state)
            {
            case SEMAPHORE_RED:
                draw_walking_pedestrian(); // Desenha o pedestre caminhando
                break;
            case SEMAPHORE_YELLOW:
                draw_standing_pedestrian(); // Desenha o pedestre parado
                break;
            case SEMAPHORE_GREEN:
                draw_standing_pedestrian(); // Desenha o pedestre parado
                break;
            default:
                break;
            }
        }
        else
        {
            draw_walking_pedestrian();
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// Tarefa para verificar o botão e alternar o modo noturno
void vCheckButtonTask()
{
    bool last_state = true;          // Considera botão inicialmente não pressionado
    uint32_t last_time_button_A = 0; // Tempo do último pressionamento

    while (1)
    {
        bool current_state = gpio_get(BUTTON_A); // Lê o estado atual do botão A

        if (last_state == true && current_state == false)
        {
            uint32_t now = get_absolute_time(); // tempo atual em ms

            if ((now - last_time_button_A) >= DEBOUNCE_DELAY) // Verifica se o tempo de debounce foi atingido
            {
                last_time_button_A = now; // Atualiza o tempo do último pressionamento
                night_mode = !night_mode; // Alterna o modo noturno
            }
        }

        last_state = current_state;    // Atualiza o estado anterior do botão
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

// Função para controlar o LED do semáforo dos pedestres
void vPedestrianTrafficLightLedTask()
{
    while (true)
    {
        if (!night_mode)
        {
            if (g_current_state == SEMAPHORE_RED)
            {
                set_led_color(GREEN); // Verde para pedestres
            }
            else
            {
                set_led_color(RED); // Vermelho para pedestres
            }
        }
        else
        {
            set_led_color(GREEN); // Verde para pedestres
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

int main()
{
    configure_leds_matrix();    // Configura a matriz de LEDs
    configure_buzzer();         // Configura o buzzer
    configure_button(BUTTON_A); // Configura o botão A
    configure_i2c_display();    // Configura o display I2C
    configure_leds();           // Configura os LEDs RGB

    // Cria as tarefas do FreeRTOS
    xTaskCreate(vTrafficLightMatrixTask, "Traffic Light Matrix Task", configMINIMAL_STACK_SIZE,
                NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(vTrafficLightBuzzerTask, "Traffic Light Buzzer Task", configMINIMAL_STACK_SIZE,
                NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(vPedestrianDisplayTask, "Pedestrian Display Task", configMINIMAL_STACK_SIZE,
                NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(vPedestrianTrafficLightLedTask, "Pedestrian Traffic Light LED Task",
                configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
    xTaskCreate(vCheckButtonTask, "Check Button Task", configMINIMAL_STACK_SIZE,
                NULL, tskIDLE_PRIORITY, NULL);

    vTaskStartScheduler(); // Inicia o agendador do FreeRTOS

    panic_unsupported(); // Se o agendador falhar, entra em um loop de erro
}