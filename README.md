# Neo Horizon Sistema de Monitoramento Orbital

[![ESP32](https://img.shields.io/badge/Hardware-ESP32-blue?style=for-the-badge&logo=espressif)](https://www.espressif.com/)
[![Wokwi Sim](https://img.shields.io/badge/Simulado-no_Wokwi-20ae5e?style=for-the-badge)](https://wokwi.com/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg?style=for-the-badge)](https://opensource.org/licenses/MIT)

Este projeto consiste em um protótipo avançado para engenharia aeroespacial baseado no microcontrolador **ESP32**. O sistema realiza a simulação de detritos orbitais e o cálculo em tempo real de riscos de colisão com satélites artificiais. 

A grande inovação da versão 2.6 é o uso de **múltiplos displays OLED (Dual-Display)** operando simultaneamente no mesmo barramento I2C, além de uma interface lógica de atuadores para simulação de injeção de falhas e evasão orbital.

---

## 🚀 Funcionalidades da Versão 2.6

* **Interface Visual Simultânea (Dual-Display):**
  * **OLED 1 (Status - `0x3C`):** Mostra continuamente a telemetria da órbita, quantidade de detritos e risco percentual de impacto.
  * **OLED 2 (Manobras - `0x3D`):** Permanece em modo *Standby* (apagado) para economizar energia do satélite. Ativa-se instantaneamente em caso de emergência, calculando e exibindo uma manobra de evasão aleatória.
* **Mecanismo de Interrupção por Hardware (Botão de Pânico):** Força um cenário crítico de colisão imediata ($90\%$ a $100\%$ de chance).
* **Feedback por LEDs Inteligentes:**
  * 🟢 **LED Verde (GPIO 18):** Órbita segura e livre de riscos iminentes.
  * 🔴 **LED Vermelho (GPIO 19):** Alerta crítico de detritos ou colisão calculada.
  * 🟡 **LED Amarelo (GPIO 25):** Ativado diretamente por hardware (botão), sinalizando o acionamento físico dos propulsores de emergência (*Hardware Override*).
* **Servidor Web e API REST:** Mantém um servidor HTTP ativo para comunicação com estações em solo.

---

## 🔌 Arquitetura de Hardware e Pinagem

Ambos os displays OLED compartilham as mesmas linhas de comunicação (**SDA** e **SCL**), mas são diferenciados via endereçamento de hardware.

| Componente | Pino do Componente | Pino do ESP32 | Função / Descrição |
| :--- | :--- | :--- | :--- |
| **OLED 1 (Status)** | SDA / SCL | **GPIO 21 / GPIO 22** | Tela de telemetria (Endereço **`0x3C`**) |
| **OLED 2 (Manobras)**| SDA / SCL | **GPIO 21 / GPIO 22** | Tela de evasão orbital (Endereço **`0x3D`**) |
| **Potenciômetro** | SIG | **GPIO 34** | Entrada Analógica - Densidade de Detritos |
| **Botão de Alerta** | Terminal 1 | **GPIO 27** | Entrada Digital (`PULLUP`) - Injeção de Risco |
| **LED Verde** | Anodo | **GPIO 18** | Indicador de Órbita Limpa / Segura |
| **LED Vermelho** | Anodo | **GPIO 19** | Indicador de Risco de Colisão / Perigo |
| **LED Amarelo** | Anodo | **GPIO 25** | Indicador de Propulsor Ativo (Botão Pressionado) |

---

## 🧠 Matriz de Decisão do Firmware

O algoritmo avalia os dados de entrada dinamicamente sem bloquear o loop principal (`millis()`):

1. **Estado de Cruzeiro (Botão Solto):**
   * O potenciômetro dita a quantidade de detritos ($10 \text{ a } 950$).
   * Se os detritos passarem de $600$, o sistema aciona o **LED Vermelho**. Caso contrário, o **LED Verde** indica estabilidade.
   * A **Tela de Manobras** fica apagada.
2. **Estado de Emergência (Botão Pressionado):**
   * O **LED Amarelo** acende instantaneamente.
   * O risco sobe para níveis críticos ($>90\%$).
   * A **Tela de Manobras** liga e escolhe aleatoriamente uma estratégia de defesa da lista:
     * *IMPULSO PROGRADO, IMPULSO RETROGRADO, AJUSTE DE INCLINAÇÃO, MUDANÇA DE ALTITUDE, MANOBRA DE APOGEU ou EVASÃO RADIAL*.

---

## 🛠️ Como Executar o Projeto no Wokwi

Para que o simulador diferencie os dois displays, o arquivo `diagram.json` deve estar configurado corretamente.

1. Baixe os arquivos `sketch.ino` e `diagram.json` deste repositório.
2. Abra o simulador [Wokwi](https://wokwi.com/) e crie um projeto para **ESP32**.
3. Substitua o código padrão do arquivo `diagram.json` pelo código fornecido neste repositório.
4. Cole o código do firmware no arquivo principal (`sketch.ino`).
5. Clique em **Iniciar Simulação (Play)**.

---

## 📝 Licença

Este projeto é distribuído sob a licença MIT. Consulte o arquivo `LICENSE` para mais detalhes.

---
*Desenvolvido como um protótipo didático para aplicação prática de barramentos industriais I2C, sistemas operacionais de tempo real e desenvolvimento de drivers embarcados.* 🚀
