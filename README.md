# Semáforo Inteligente com FreeRTOS - BitDogLab

## 👤 Autor
**Hilquias Rodrigues de Oliveira**

## 📍 Descrição do Projeto

Este projeto intrutório de tarefas com **FreeRTOS** consiste na construção de um sistema semafórico inteligente, utilizando os seguintes periféricos:

- Matriz de LEDs WS2812  
- LED RGB  
- Display OLED SSD1306  
- Botão A  
- Buzzer piezoelétrico  

O comportamento do sistema varia conforme o modo de operação (normal ou noturno), alternável por meio do botão A.

## 🎥 Vídeo de Funcionamento

https://www.youtube.com/watch?v=alNAH6l1urg

## ⚙️ Instruções de Compilação

Certifique-se de ter o ambiente devidamente configurado:

1. Pico SDK instalado e configurado.  
2. VSCode com extensões necessárias (CMake, Cortex-Debug, Pico SDK).  
3. Clone este repositório e abra a pasta do projeto.  
4. A extensão Pi Pico criará automaticamente a pasta `build`.  
5. Clique em **Compile** na barra inferior direita do VSCode.  
6. O arquivo `.uf2` será gerado.  
7. Coloque a placa BitDogLab em modo **BOOTSEL**.  
8. Arraste o `.uf2` até a unidade correspondente à placa.  
9. O programa será executado automaticamente.

## 🧠 Manual de Funcionamento

### 🔁 Modo Normal

#### Matriz de LEDs WS2812
Representa o semáforo de veículos.  
Exibe as cores:
- **Vermelho** por 10 s  
- **Amarelo** por 4 s  
- **Verde** por 10 s  

A cor ativa brilha intensamente, as demais têm brilho reduzido.

#### 🔊 Buzzer
Emite sinais sonoros conforme o estado:

- **Vermelho**: Beep longo de 0,5 s + 1,5 s de silêncio  
- **Amarelo**: Beeps curtos de 0,1 s com pausas de 0,1 s  
- **Verde**: Beep curto de 0,1 s + 0,9 s de silêncio  

#### 🖥️ Display OLED (SSD1306)
Exibe ícones de pedestre:

- **“GO”** com pedestre caminhando quando o sinal estiver **vermelho**  
- **“STOP”** com pedestre parado quando estiver **amarelo** ou **verde**

#### 💡 LED RGB
Simula o semáforo de pedestres:

- **Verde**: Travessia segura (semáforo de veículos vermelho)  
- **Vermelho**: Travessia não permitida (veículos com sinal verde ou amarelo)

### 🌙 Modo Noturno (ativado ao pressionar botão A)

#### Matriz de LEDs
Cor **amarela** permanece acesa continuamente como alerta.

#### 🔊 Buzzer
Beep suave de **1 s** com pausa de **2 s**.

#### 🖥️ Display OLED
Mostra pedestre caminhando com a mensagem **“GO”**.

#### 💡 LED RGB
Permanece **verde**, sinalizando que a travessia está sempre permitida.
