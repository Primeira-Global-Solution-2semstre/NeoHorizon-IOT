# NeoHorizon Sistema de Monitoramento Orbital

[![ESP32](https://img.shields.io/badge/Hardware-ESP32-blue?style=for-the-badge&logo=espressif)](https://www.espressif.com/)
[![Wokwi Sim](https://img.shields.io/badge/Simulado-no_Wokwi-20ae5e?style=for-the-badge)](https://wokwi.com/projects/466283893846870017)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg?style=for-the-badge)](https://opensource.org/licenses/MIT)

Este projeto consiste em um protótipo avançado de Internet das Coisas (IoT) desenvolvido com o microcontrolador **ESP32** para a **Global Solution 2026**. O sistema realiza a simulação de detritos orbitais e o cálculo em tempo real de riscos de colisão com satélites artificiais, visando a sustentabilidade e segurança da infraestrutura aeroespacial.

A grande inovação da versão 2.6 é o uso de **múltiplos displays OLED (Dual-Display)** operando simultaneamente no mesmo barramento I2C, além de uma interface lógica de atuadores para simulação de injeção de falhas e evasão orbital.

---

## 👥 Integrantes do Grupo

* **RAFAEL KUBAGAWA RAMOS** - RM: 565572
* **VINICIUS SOTERAS BRAGA** - RM: 566230
  
---

## 📺 Apresentação e Demonstração do Projeto

Assista ao vídeo explicativo de até 3 minutos contendo a proposta da solução, arquitetura técnica e a demonstração prática do circuito funcionando no simulador:

▶️ **[Clique aqui para assistir ao vídeo no YouTube](SINAL_DE_MAIOR_LINK_DO_YOUTUBE_AQUI)**

---

## 🚀 Funcionalidades da Versão 2.6

* **Interface Visual Simultânea (Dual-Display):**
  * **OLED 1 (Status - `0x3C`):** Mostra continuamente a telemetria da órbita, quantidade de detritos e risco percentual de impacto.
  * **OLED 2 (Manobras - `0x3D`):** Permanece em modo *Standby* (apagado). Ativa-se instantaneamente em caso de emergência, calculando e exibindo uma manobra de evasão aleatória.
* **Mecanismo de Interrupção por Hardware (Botão de Pânico):** Força um cenário crítico de colisão imediata (90% a 100% de chance).
* **Feedback por LEDs Inteligentes:**
  * 🟢 **LED Verde (GPIO 18):** Órbita segura e livre de riscos iminentes.
  * 🔴 **LED Vermelho (GPIO 19):** Alerta crítico de detritos ou colisão calculada.
  * 🟡 **LED Amarelo (GPIO 25):** Ativado diretamente por hardware (botão), sinalizando o acionamento físico dos propulsores de emergência (*Hardware Override*).
* **Servidor Web e API REST:** Mantém um servidor HTTP ativo para comunicação com estações em solo.

---

## 🔌 Arquitetura de Hardware e Pinagem

Ambos os displays OLED compartilham as mesmas linhas de comunicação (**SDA** e **SCL**), mas são diferenciados via endereçamento de hardware interno no simulador.

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

O algoritmo avalia os dados de entrada dinamicamente sem bloquear o loop principal:

1. **Estado de Cruzeiro (Botão Solto):**
   * O potenciômetro dita a quantidade de detritos (10 a 950).
   * Se os detritos passarem de 600, o sistema aciona o **LED Vermelho**. Caso contrário, o **LED Verde** indica estabilidade.
   * A **Tela de Manobras** fica apagada por eficiência energética.
2. **Estado de Emergência (Botão Pressionado):**
   * O **LED Amarelo** acende instantaneamente.
   * O risco sobe para níveis críticos (>90%).
   * A **Tela de Manobras** liga e escolhe aleatoriamente uma estratégia de defesa da lista (*IMPULSO PROGRADO, IMPULSO RETROGRADO, AJUSTE DE INCLINAÇÃO, MUDANÇA DE ALTITUDE, MANOBRA DE APOGEU ou EVASÃO RADIAL*).

---

## 🛠️ Como Executar a Simulação

Este projeto está totalmente configurado e pronto para ser executado de forma 100% online:

1. Acesse o link direto do ambiente de desenvolvimento: **[Projeto no Wokwi](https://wokwi.com/projects/466283893846870017)**.
2. Clique no botão **Play / Start Simulation**.
3. Interaja com o potenciômetro e o botão na interface gráfica para observar a resposta imediata dos displays e dos LEDs.
