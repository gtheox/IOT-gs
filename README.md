# Pluviômetro IoT com ESP32 e ThingSpeak 🌧️📡

Este projeto simula um pluviômetro digital com ESP32 e envia os dados de precipitação para o ThingSpeak via HTTP. A contagem de pulsos do Reed Switch é usada para calcular a quantidade de chuva em mm.

## 📷 Visão do Circuito

![Esquema de Ligações](wokwi-diagrama.png)

## ⚙️ Componentes Utilizados

- ESP32 Devkit V1
- Reed Switch
- Resistor de 10kΩ (pull-down)
- LCD 16x2 com módulo I2C
- Jumpers e Protoboard

## 🔧 Montagem no Wokwi

O circuito foi simulado no [Wokwi](https://wokwi.com/):

- **Reed Switch** no GPIO16 com `INPUT_PULLUP`
- **LCD I2C**: SDA no GPIO21, SCL no GPIO22

## ☁️ Conexão com ThingSpeak

Os dados são enviados via HTTP a cada 60 segundos para os campos:

- `field1`: Número de pulsos (contagem do Reed)
- `field2`: Milímetros de chuva (`REEDCOUNT * 0.25`)

## 📡 Configuração Wi-Fi

No código, substitua:

```cpp
const char* ssid = "SEU_SSID";
const char* password = "SUA_SENHA";
```

## 🔗 Configuração do ThingSpeak

- Crie um canal com pelo menos 2 campos
- Copie a **Write API Key** e **Channel ID**
- Substitua no código:

```cpp
const unsigned long thingspeakChannelID = SEU_CHANNEL_ID;
const char* thingspeakApiKey = "SUA_WRITE_API_KEY";
```

## ▶️ Simulação no Wokwi

- Clique no Reed Switch para simular chuva
- Os dados aparecem no LCD e Serial Monitor
- Após 60s, são enviados ao ThingSpeak

## 🎥 Demonstração em Vídeo

[🔗 Link para o vídeo explicativo (YouTube)](https://youtu.be/ZbSAIAa-pE8)

---
