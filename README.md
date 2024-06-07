# OceanGuard

OceanGuard é um projeto de monitoramento oceânico que mede a temperatura, o pH e a luminosidade da água. Com base nas variações detectadas pelos sensores, ele envia alertas para os usuários.

## Funcionalidades

- **Medição de Temperatura:** Utiliza um termistor para medir a temperatura da água.
- **Medição de pH:** Mede o valor de pH da água e aciona LEDs e servos correspondentes para indicar o nível de acidez ou alcalinidade.
- **Medição de Luminosidade:** Utiliza um sensor LDR para medir a luminosidade abaixo da água.
- **Alertas:** Emite alertas sonoros quando a luminosidade ultrapassa um determinado limiar.
- **Indicadores Visuais:** LEDs indicam diferentes níveis de pH.
- **Controle de Servos:** Ajusta a posição de servos baseados nos valores de pH medidos.

## Hardware Necessário

- ESP32 ou similar
- LEDs de diferentes cores
- Servos
- Sensor de temperatura (termistor)
- Sensor de pH
- Sensor LDR
- Buzzer
- Resistores
- Cabos e protoboard

## Conexões de Hardware

- **LEDs:** Conecte os LEDs nos pinos especificados no código (`RLED`, `OLED`, `YLED`, etc.).
- **Sensores:**
  - Termistor: Conecte ao pino analógico `32`.
  - LDR: Conecte ao pino analógico `15`.
  - pH: Conecte ao pino analógico `POTPIN` (12).
- **Servos:** Conecte os servos nos pinos `alkalinePin` (26), `neutralPin` (25) e `acidicPin` (27).
- **Buzzer:** Conecte o buzzer ao pino `buzzerAlarm` (33).

## Instalação

1. **Configuração do Ambiente de Desenvolvimento:**
   - Instale a IDE Arduino.
   - Adicione o suporte para ESP32 na IDE Arduino.

2. **Bibliotecas Necessárias:**
   - `ArduinoJson.h`
   - `WiFi.h`
   - `WiFiClient.h`
   - `PubSubClient.h`

3. **Código:**
   - Carregue o código fornecido na placa ESP32 através da IDE Arduino.

## Como Usar

1. **Ligue a Placa:** Conecte a placa ESP32 ao seu computador via USB.
2. **Carregue o Código:** Utilize a IDE Arduino para carregar o código na placa.
3. **Monitore os Dados:** Abra o monitor serial na IDE Arduino para visualizar os valores de pH, temperatura e luminosidade.
4. **Responda aos Alertas:** Observe os LEDs e ouça o buzzer para quaisquer alertas que possam indicar variações significativas nos sensores.
