#include <ArduinoJson.h> // Biblioteca para manipulação de JSON
#include <WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <driver/ledc.h>

// Definições de pinos dos LEDs e sensores
#define RLED 23 
#define OLED 22  
#define YLED 21 
#define LGLED 19
#define DGLED 18  
#define LBLED  5  
#define DBLED 4  
#define LPLED 2  
#define DPLED 13  
#define POTPIN 12

// Define servo pins
#define alkalinePin 26
#define neutralPin 25
#define acidicPin 27

// Define servo positions
int servoMin = 1000;   // Minimum pulse width (microseconds)
int servoMax = 2000;   // Maximum pulse width (microseconds)
int servoCenter = (servoMin + servoMax) / 2; // Center pulse width (microseconds)

int sensorLDR = 36; // defining SensorLDR pin
int buzzerAlarm = 33; // defining Buzzer pin
int Temp = 32; // defining Temp pin


// Definições dos parâmetros para o sensor de temperatura
const float BETA = 3950; 


// Credenciais de WiFi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Detalhes do broker MQTT
const char* mqtt_server = "broker.hivemq.com"; 
const int mqtt_port = 1883;
const char* mqtt_topic = "fiap/oceanGuard"; 

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);

  // Configuração dos pinos
  pinMode(RLED, OUTPUT);
  pinMode(OLED, OUTPUT);
  pinMode(YLED, OUTPUT);
  pinMode(LGLED, OUTPUT);
  pinMode(DGLED, OUTPUT);
  pinMode(LBLED, OUTPUT);
  pinMode(DBLED, OUTPUT);
  pinMode(LPLED, OUTPUT);
  pinMode(DPLED, OUTPUT);
  
  pinMode(POTPIN, INPUT);

  pinMode(sensorLDR, INPUT);

  pinMode(alkalinePin, OUTPUT);
  pinMode(neutralPin, OUTPUT);
  pinMode(acidicPin, OUTPUT);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conexão MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("Conectado");
    } else {
      Serial.print("Falha, código de retorno = ");
      Serial.print(client.state());
      Serial.println(", tentando novamente em 2 segundos");
      delay(2000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Leitura dos sensores
  float pHValue = map(analogRead(POTPIN), 0, 4095, 0.0, 14.0);  // Map analog reading to pH range 0-14
  Serial.print("pH Value: ");
  Serial.println(pHValue, 2); // Print pH value with 2 decimal places 

  // Controle dos LEDs baseado no valor do pH
  digitalWrite(RLED, pHValue == 0);
  digitalWrite(OLED, pHValue >= 1.0 && pHValue <= 2.0);
  digitalWrite(YLED, pHValue >= 3.0 && pHValue <= 4.0);
  digitalWrite(LGLED, pHValue >= 5.0 && pHValue <= 6.0);
  digitalWrite(DGLED, pHValue == 7.0);
  digitalWrite(LBLED, pHValue >= 8.0 && pHValue <= 9.0);
  digitalWrite(DBLED, pHValue >= 10.0 && pHValue <= 11.0);
  digitalWrite(LPLED, pHValue >= 12.0 && pHValue <= 13.0);
  digitalWrite(DPLED, pHValue == 14.0);

  // Map pH value to servo positions
  int alkalinePosition = map(pHValue, 1.0, 3.0, servoMin, servoMax);
  int neutralPosition = map(pHValue, 4.0, 5.2, servoMin, servoMax);
  int acidicPosition = map(pHValue, 6.5, 14.0, servoMin, servoMax);

  // Control the servos using PWM signals
  if (pHValue >= 1.0 && pHValue <= 3.0) {
    generatePWM(alkalinePin, alkalinePosition);
  } else {
    generatePWM(alkalinePin, servoCenter);
  }

  if (pHValue >= 4.0 && pHValue <= 5.2) {
    generatePWM(neutralPin, neutralPosition);
  } else {
    generatePWM(neutralPin, servoCenter);
  }

  if (pHValue >= 6.5 && pHValue <= 14.0) {
    generatePWM(acidicPin, acidicPosition);
  } else {
    generatePWM(acidicPin, servoCenter);
  }

  int luminosidade = analogRead(sensorLDR);
  float light = luminosidade/100;
  Serial.print("Luz em metros: ");
  Serial.println(light);

  int analogValue = analogRead(Temp);
  float celsius = 1 / (log(1 / (4095.0 / analogValue - 1)) / BETA + 1.0 / 298.15) - 273.15;

  char celsiusFormatted[6]; // buffer para armazenar o valor formatado
  dtostrf(celsius, 4, 1, celsiusFormatted); // formata o valor com 4 dígitos no total, incluindo 1 decimal

  Serial.print("Temperatura (°C): ");
  Serial.println(celsiusFormatted); // Imprime o valor formatado



  // Envio dos dados via MQTT em formato JSON
  StaticJsonDocument<200> doc;
  doc["pH"] = pHValue;
  doc["Luminosidade"] = light;
  doc["Temperatura"] = celsiusFormatted;

  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer);
  client.publish(mqtt_topic, jsonBuffer);

  delay(3000);
}

void generatePWM(int pin, int pulseWidth) {
  digitalWrite(pin, HIGH);
  delayMicroseconds(pulseWidth);
  digitalWrite(pin, LOW);
  delayMicroseconds(20000 - pulseWidth); // 20ms period
}

