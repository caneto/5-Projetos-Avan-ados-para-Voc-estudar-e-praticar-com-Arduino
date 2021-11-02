#include <RH_NRF24.h>
#include "DHT.h"
#include <EEPROM.h>
 
RH_NRF24 nrf24(2, 4); // use isso para NodeMCU Amica / AdaFruit Huzzah ESP8266 Feather
// RH_NRF24 nrf24(8, 7); // use isso com Arduino UNO / Nano

#define DHTPIN D1
//#define DHTPIN 2
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);
int deviceID = EEPROM.read(0);
int humidity, temperature;
 
void setup()
{
  Serial.begin(115200);
  dht.begin();
  while (!Serial); // aguarde a conexão da porta serial. Necessário apenas para Leonardo
  if (!nrf24.init()) 
  {
    Serial.println("init failed");
  }
  // Os padrões após o init são 2,402 GHz (canal 2), 2Mbps, 0dBm
  if (!nrf24.setChannel(3)) 
  {
    Serial.println("setChannel falhou");
  }
  if (!nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm)) {
    Serial.println("setRF falhou");
  }
  Serial.println("Transmissor iniciado");
}
 
void loop() 
{
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  if (isnan(humidity) || isnan(temperature))  {
    Serial.println(F("Falha ao ler do sensor DHT!"));
    return;
  }
  Serial.println("Enviando para o gateway");
  uint8_t data[4];
  
  data[0] = humidity;
  data[1] = temperature;
  data[2] = deviceID;
  Serial.println("------------- Medidas -------------");
  Serial.print("Humidade: ");
  Serial.print(data[0]);
  
  Serial.print(", Temperatura: ");
  Serial.print(data[1]);
 
  Serial.print(", ID: ");
  Serial.print(data[2]);
  Serial.println();
 
  nrf24.send(data, sizeof(data));
  nrf24.waitPacketSent();
  
  // Agora aguarde uma resposta
  uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
 
  if (nrf24.waitAvailableTimeout(1000))
  {
    // Deve ser uma mensagem de resposta para nós agora
    if (nrf24.recv(buf, &len))    {
      Serial.print("obteve resposta: ");
      Serial.println((char*)buf);
    } else {
        Serial.println("recepcao falhou");
    }
  } else {
    Serial.println("Sem resposta.");
  }
  delay(5000);
}
