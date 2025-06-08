#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>

// Configurações da rede Wi-Fi
const char* ssid = "loirinhoshibuya";
const char* password = "ilhadoqueijo";

// Configurações do ThingSpeak
const char* thingspeakHost = "api.thingspeak.com";
const unsigned long thingspeakChannelID = 2983679;
const char* thingspeakApiKey = "YAJ133RIM122UJAX";

// Inicialização do LCD (endereço I2C padrão)
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Pino do Reed Switch
const int REED = 16;

// Variáveis de contagem de pulsos e tempo
volatile int REEDCOUNT = 0;
unsigned long lastSendTime = 0;
const long sendInterval = 60000; // Envia dados a cada 60 segundos

// Interrupção que conta pulsos do Reed Switch
void IRAM_ATTR handleReedInterrupt() {
  REEDCOUNT++;
}

void setup() {
  Serial.begin(115200);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Pulsos: ");
  lcd.setCursor(0, 1);
  lcd.print("Chuva: ");

  // Conexão Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  pinMode(REED, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(REED), handleReedInterrupt, FALLING);
}

void loop() {
  float chuva_mm = REEDCOUNT * 0.25;

  // Exibe no Serial Monitor
  Serial.printf("Pulsos: %d | Chuva: %.2f mm\n", REEDCOUNT, chuva_mm);

  // Exibe no LCD
  lcd.setCursor(8, 0);
  lcd.print(REEDCOUNT);
  lcd.print("   ");
  lcd.setCursor(8, 1);
  lcd.print(chuva_mm);
  lcd.print(" mm   ");

  // Envia dados ao ThingSpeak
  if (millis() - lastSendTime > sendInterval) {
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String data = "api_key=" + String(thingspeakApiKey) +
                    "&field1=" + String(REEDCOUNT) +
                    "&field2=" + String(chuva_mm);

      http.begin("http://" + String(thingspeakHost) + "/update");
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      int responseCode = http.POST(data);

      if (responseCode > 0) {
        Serial.printf("HTTP: %d\n", responseCode);
      } else {
        Serial.printf("Erro HTTP: %d\n", responseCode);
      }

      http.end();
    } else {
      Serial.println("Wi-Fi desconectado. Reconectando...");
      WiFi.reconnect();
    }

    lastSendTime = millis();
    REEDCOUNT = 0;
  }

  delay(1000);
}
