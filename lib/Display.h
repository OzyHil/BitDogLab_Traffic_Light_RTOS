#ifndef DISPLAY_H
#define DISPLAY_H

// Inclusão das bibliotecas necessárias
#include "General.h"     // Definições gerais do sistema
#include "ssd1306.h"     // Controle do display OLED

// Definição de pinos e endereço do display I2C
#define I2C_PORT i2c1            // Porta I2C para comunicação
#define I2C_SDA 14               // Pino SDA para I2C
#define I2C_SCL 15               // Pino SCL para I2C
#define DISPLAY_ADDRESS 0x3C     // Endereço do display OLED


void configure_i2c_display(); // Função para configurar o display I2C

void clear_i2c_display(); // Função para limpar o display OLED

void draw_walking_pedestrian(); // Função para desenhar um pedestre caminhando

void draw_standing_pedestrian(); // Função para desenhar um pedestre parado

#endif