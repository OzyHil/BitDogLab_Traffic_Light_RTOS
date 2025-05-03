#include "Buzzer.h"  // Inclusão do cabeçalho com definições do buzzer

// Função para configurar o buzzer utilizando PWM
void configure_buzzer()
{
    // Inicializa o PWM no pino do buzzer (BUZZER_A) com valor de "wrap" especificado
    init_pwm(BUZZER_A, WRAP_PWM_BUZZER);
}

// Função para definir o nível (intensidade) do sinal PWM no pino do buzzer
void set_buzzer_level(uint gpio, uint16_t level)
{
    // Obtém o número do slice PWM associado ao pino especificado
    uint slice_num = pwm_gpio_to_slice_num(gpio);

    // Define o nível do canal PWM correspondente ao pino (duty cycle)
    pwm_set_chan_level(slice_num, pwm_gpio_to_channel(gpio), level);
}

// Função para ativar o buzzer em modo de alerta (beep) vermelho
void beep_mode_red()
{ 
    set_buzzer_level(BUZZER_A, WRAP_PWM_BUZZER / 90);
    vTaskDelay(pdMS_TO_TICKS(500));
    set_buzzer_level(BUZZER_A, 0);
    vTaskDelay(pdMS_TO_TICKS(1500));
}

// Beep rápido intermitente “atenção”
void beep_mode_yellow()
{
    for (int i = 0; i < 5; i++)
    {
        set_buzzer_level(BUZZER_A, WRAP_PWM_BUZZER / 90);
        vTaskDelay(pdMS_TO_TICKS(100));
        set_buzzer_level(BUZZER_A, 0);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// 1 beep curto por um segundo “pode atravessar”
void beep_mode_green()
{
    set_buzzer_level(BUZZER_A, WRAP_PWM_BUZZER / 90);
    vTaskDelay(pdMS_TO_TICKS(100));
    set_buzzer_level(BUZZER_A, 0);
    vTaskDelay(pdMS_TO_TICKS(900));
}
// Em modo noturno, bipa levemente
void beep_mode_night()
{
    set_buzzer_level(BUZZER_A, WRAP_PWM_BUZZER / 90);
    vTaskDelay(pdMS_TO_TICKS(1000));
    set_buzzer_level(BUZZER_A, 0);
    vTaskDelay(pdMS_TO_TICKS(2000));
}
