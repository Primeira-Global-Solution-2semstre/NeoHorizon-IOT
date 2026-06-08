#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

// INSTANCIAÇÃO DOS DOIS DISPLAYS
Adafruit_SSD1306 displayStatus(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Adafruit_SSD1306 displayManobras(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// PINOS DO HARDWARE
const int PIN_POT = 34;
const int PIN_BUTTON = 27;
const int PIN_LED_GREEN = 18;
const int PIN_LED_RED = 19;
const int PIN_LED_YELLOW = 25;

WebServer server(80);

// ESTADOS DO SISTEMA
int altitudeRaw = 0;
int qtdDetritos = 0;
float probabilidadeColisao = 0.0;
String acaoManobra = "";
bool emergencyActive = false;
bool ultimoEstadoBotao = false;

// Banco de manobras aleatórias
const char* LISTA_MANOBRAS[] = {
  "IMPULSO PROGRADO",
  "IMPULSO RETROGRADO",
  "AJUSTE DE INCLINACAO",
  "MUDANCA DE ALTITUDE",
  "MANOBRA DE APOGEU",
  "EVASAO RADIAL"
};

void setup() {
  Serial.begin(115200);
  pinMode(PIN_POT, INPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_YELLOW, OUTPUT);

  // Inicializa o Display 1 (Status) no endereço 0x3C
  if(!displayStatus.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("Falha no OLED de Status (0x3C)!"));
    for(;;);
  }
  
  // Inicializa o Display 2 (Manobras) no endereço 0x3D
  if(!displayManobras.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { 
    Serial.println(F("Falha no OLED de Manobras (0x3D)!"));
    for(;;);
  }
  
  // Configurações iniciais de texto
  displayStatus.setTextSize(1);
  displayStatus.setTextColor(SSD1306_WHITE);
  displayManobras.setTextSize(1);
  displayManobras.setTextColor(SSD1306_WHITE);

  // Conexão WiFi padrão Wokwi
  WiFi.begin("Wokwi-GUEST", "");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Conectado!");

  server.on("/", [](){ server.send(200, "text/html", "<h1>Monitor Orbital Ativo</h1>"); });
  server.begin();
}

void atualizarDados() {
  altitudeRaw = analogRead(PIN_POT);
  bool botaoPressionado = (digitalRead(PIN_BUTTON) == LOW); // INPUT_PULLUP
  
  // LED Amarelo acompanha o botão perfeitamente
  digitalWrite(PIN_LED_YELLOW, botaoPressionado ? HIGH : LOW);

  // Mapeamento básico de detritos pelo potenciômetro
  qtdDetritos = map(altitudeRaw, 0, 4095, 10, 950);

  // Detecção de borda de subida (Momento exato em que o botão é APERTADO)
  if (botaoPressionado && !ultimoEstadoBotao) {
    emergencyActive = true;
    // Sorteia uma manobra diferente de forma síncrona com o clique do botão
    int indiceAleatorio = random(0, 6);
    acaoManobra = LISTA_MANOBRAS[indiceAleatorio];
  } 
  else if (!botaoPressionado) {
    emergencyActive = false;
  }
  
  ultimoEstadoBotao = botaoPressionado;

  // Lógica de cálculo de risco e LEDs de alerta baseados nos detritos/botão
  if (emergencyActive || qtdDetritos > 600) {
    probabilidadeColisao = emergencyActive ? random(900, 1000) / 10.0 : random(50, 89);
    digitalWrite(PIN_LED_RED, HIGH);
    digitalWrite(PIN_LED_GREEN, LOW);
  } else {
    probabilidadeColisao = map(qtdDetritos, 10, 600, 0, 49);
    digitalWrite(PIN_LED_RED, LOW);
    digitalWrite(PIN_LED_GREEN, HIGH);
  }
}

void renderizarDisplayStatus() {
  displayStatus.clearDisplay();
  displayStatus.setCursor(0, 0);
  displayStatus.println("--- MONITOR ORBITAL ---");
  displayStatus.println("-----------------------");
  
  displayStatus.print("Detritos: "); 
  displayStatus.print(qtdDetritos); 
  displayStatus.println(" em orbita");
  
  displayStatus.println();
  displayStatus.print("Risco Colisao: "); 
  displayStatus.print(probabilidadeColisao, 1); 
  displayStatus.println("%");
  
  displayStatus.display();
}

void renderizarDisplayManobras() {
  displayManobras.clearDisplay();
  
  // Só desenha algo se o botão estiver ativamente pressionado
  if (emergencyActive) {
    displayManobras.setCursor(0, 0);
    displayManobras.println("      ¡ALERTA!     ");
    displayManobras.println("-----------------------");
    displayManobras.println("POSSIVEL COLISAO");
    displayManobras.println("IMINENTE!");
    displayManobras.println();
    displayManobras.print("MANOBRA: ");
    displayManobras.println(acaoManobra);
  }
  
  // Se o botão não estiver pressionado, displayManobras.clearDisplay() garante tela apagada
  displayManobras.display();
}

void loop() {
  server.handleClient();
  atualizarDados();

  // Renderização simultânea e em tempo real nas duas telas
  renderizarDisplayStatus();
  renderizarDisplayManobras();

  delay(30); // Estabilidade da simulação
}