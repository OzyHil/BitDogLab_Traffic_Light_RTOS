#include "Display.h" // Inclusão do cabeçalho com definições relacionadas ao display OLED

ssd1306_t ssd;   // Estrutura que representa o display OLED SSD1306
bool cor = true; // Variável para armazenar a cor do display (true ou false)

// Função para configurar a comunicação I2C e inicializar o display OLED
void configure_i2c_display()
{
    // Inicializa o barramento I2C na porta e com frequência de 400 kHz
    i2c_init(I2C_PORT, 400 * 1000);

    // Define as funções dos pinos SDA e SCL como I2C
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);

    // Ativa resistores de pull-up nos pinos SDA e SCL
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Inicializa o display com resolução 128x64, sem rotação, endereço I2C e porta definida
    ssd1306_init(&ssd, 128, 64, false, DISPLAY_ADDRESS, I2C_PORT);

    // Configura o display com parâmetros padrão
    ssd1306_config(&ssd);

    // Envia os dados iniciais para o display
    ssd1306_send_data(&ssd);
}

// Função para limpar o display OLED
void clear_i2c_display()
{
    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
}

void draw_walking_pedestrian()
{

    ssd1306_fill(&ssd, false); // Limpa o display antes de desenhar o pedestre

    ssd1306_draw_string(&ssd, "GO", 8, 28); // Desenha o texto "Go" no display
    ssd1306_draw_string(&ssd, "GO", 104, 28);

    ssd1306_rect(&ssd, 2, 36, 54, 62, cor, !cor); // Desenha um retângulo respresentando a borda do desenho

    //======== Cabeça ========//
    // topo
    ssd1306_hline(&ssd, 62, 64, 9, cor);

    // face esquerda
    ssd1306_line(&ssd, 62, 9, 59, 12, cor);
    ssd1306_vline(&ssd, 59, 12, 15, cor);
    ssd1306_line(&ssd, 59, 15, 62, 18, cor);

    // face direita
    ssd1306_line(&ssd, 65, 9, 67, 12, cor);
    ssd1306_vline(&ssd, 67, 12, 15, cor);
    ssd1306_line(&ssd, 67, 15, 65, 18, cor);

    ssd1306_hline(&ssd, 62, 64, 18, cor); // queixo

    //======== Ombro ========//
    ssd1306_line(&ssd, 58, 20, 70, 20, true);

    //======== Tronco ========//
    ssd1306_vline(&ssd, 58, 20, 36, true); // lado esquerdo
    ssd1306_vline(&ssd, 70, 26, 36, true); // lado direito

    //======== Braço Esquerdo ========//
    ssd1306_line(&ssd, 58, 26, 50, 32, true);
    ssd1306_line(&ssd, 50, 32, 46, 32, true);
    ssd1306_line(&ssd, 46, 32, 44, 31, true);

    ssd1306_line(&ssd, 58, 20, 50, 28, true);
    ssd1306_line(&ssd, 50, 28, 46, 28, true);
    ssd1306_line(&ssd, 46, 28, 44, 30, true);

    //======== Braço Direito ========//
    ssd1306_line(&ssd, 70, 26, 80, 34, true);
    ssd1306_line(&ssd, 70, 20, 82, 30, true);
    ssd1306_line(&ssd, 82, 30, 83, 33, true);
    ssd1306_line(&ssd, 81, 34, 83, 33, true);

    //======== Perna Esquerda ========//
    ssd1306_line(&ssd, 58, 36, 44, 54, true);
    ssd1306_line(&ssd, 64, 40, 50, 58, true);

    ssd1306_line(&ssd, 44, 55, 45, 58, true);
    ssd1306_hline(&ssd, 46, 49, 58, true); // pé

    //======== Perna Direita ========//
    ssd1306_line(&ssd, 64, 40, 76, 58, true);
    ssd1306_line(&ssd, 70, 36, 82, 54, true);

    ssd1306_line(&ssd, 82, 55, 80, 58, true);
    ssd1306_hline(&ssd, 77, 79, 58, true); // pé

    // Atualiza o display
    ssd1306_send_data(&ssd);
}

void draw_standing_pedestrian()
{

    ssd1306_fill(&ssd, false);

    ssd1306_draw_string(&ssd, "STOP", 1, 28); 
    ssd1306_draw_string(&ssd, "STOP", 94, 28);

    ssd1306_rect(&ssd, 2, 36, 54, 62, cor, !cor); 

    //======== Cabeça ========//
    ssd1306_hline(&ssd, 62, 66, 9, cor); // topo

    // face esquerda
    ssd1306_line(&ssd, 62, 9, 59, 12, cor);
    ssd1306_vline(&ssd, 59, 12, 15, cor);
    ssd1306_line(&ssd, 59, 15, 62, 18, cor);

    // face direita
    ssd1306_line(&ssd, 66, 9, 69, 12, cor);
    ssd1306_vline(&ssd, 69, 12, 15, cor);
    ssd1306_line(&ssd, 69, 15, 66, 18, cor);

    ssd1306_hline(&ssd, 62, 66, 18, cor); // queixo

    //======== Ombro ========//
    ssd1306_line(&ssd, 58, 20, 70, 20, true);

    // Tronco
    ssd1306_vline(&ssd, 58, 26, 36, true);
    ssd1306_vline(&ssd, 70, 26, 36, true);

    //======== Braço Esquerdo ========//
    ssd1306_line(&ssd, 58, 20, 46, 30, true); // cima
    ssd1306_line(&ssd, 58, 26, 48, 34, true); // baixo

    ssd1306_line(&ssd, 46, 30, 45, 33, true); // mão cima
    ssd1306_line(&ssd, 47, 34, 45, 33, true); // mão baixo

    //======== Braço Direito ========//
    ssd1306_line(&ssd, 70, 20, 82, 30, true); // cima
    ssd1306_line(&ssd, 70, 26, 80, 34, true); // baixo

    ssd1306_line(&ssd, 82, 30, 83, 33, true); // mão cima
    ssd1306_line(&ssd, 81, 34, 83, 33, true); // mão baixo

    //======== Perna Esquerda ========//
    ssd1306_line(&ssd, 58, 36, 52, 54, true); // fora
    ssd1306_line(&ssd, 64, 40, 60, 54, true); // dentro

    ssd1306_line(&ssd, 52, 55, 54, 58, true);
    ssd1306_line(&ssd, 60, 55, 58, 58, true);
    
    // pé
    ssd1306_hline(&ssd, 55, 57, 58, true); 

    //======== Perna Direita ========//
    ssd1306_line(&ssd, 64, 40, 68, 54, true); // dentro
    ssd1306_line(&ssd, 70, 36, 76, 54, true); // fora

    ssd1306_line(&ssd, 68, 55, 70, 58, true);
    ssd1306_line(&ssd, 76, 55, 74, 58, true);
    
    // pé
    ssd1306_hline(&ssd, 71, 73, 58, true); 

    // Atualiza o display
    ssd1306_send_data(&ssd);
}