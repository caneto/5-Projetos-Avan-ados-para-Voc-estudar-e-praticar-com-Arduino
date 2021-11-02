#include <RH_NRF24.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
 
String apiKey = "KBD1JSZTUKCXJ15V";
 
const char* ssid = "Alexahome";
const char* password = "loranthus";
 
WiFiClient client;
int gatewayID = EEPROM.read(0);
 
const char* server = "api.thingspeak.com";
 
RH_NRF24 nrf24(2, 4); // use isso para NodeMCU Amica/AdaFruit Huzzah ESP8266 Feather
 
void setup()
{
  Serial.begin(115200);
  Serial.print("Receiver Started, ID: ");
  Serial.print("Conectando à ");
  Serial.println(ssid);
  Serial.print(gatewayID);
  Serial.println();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi conectado");
  nrf24.init();
  nrf24.setChannel(3);
  nrf24.setRF(RH_NRF24::DataRate2Mbps, RH_NRF24::TransmitPower0dBm);
}
 
void loop()
{
  if (nrf24.available())
  {
    // Deve ser uma mensagem para nós agora
    uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (nrf24.recv(buf, &len))
    {
       // Envie uma resposta
      uint8_t sdata[] = "Informação recebida.";
      nrf24.send(sdata, sizeof(sdata));
      nrf24.waitPacketSent();
 
      int humidity = buf[0];
      int temperature = buf[1];
      int deviceID = buf[2];
 
      Serial.println("--- Dados recuperados do dispositivo ---");
      if (client.connect(server, 80)) { // "184.106.153.149" or api.thingspeak.com
        String postStr = apiKey;
        postStr += "&field1=";
        postStr += String(temperature);
        postStr += "&field2=";
        postStr += String(humidity);
        postStr += "&field3=";
        postStr += String(gatewayID);
        postStr += "&field4=";
        postStr += String(deviceID);
        postStr += "\r\n\r\n";
        
        client.print("POST /update HTTP/1.1\n");
        client.print("Host: api.thingspeak.com\n");
        client.print("Connection: close\n");
        client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");   client.print(postStr.length());
        client.print("\n\n");   client.print(postStr);
        
        Serial.println("---- Dados enviados para Thingspeak ----");
        Serial.print("Device ID: ");      Serial.print(deviceID);
        Serial.print(", Temperatura:");    Serial.print(temperature);
        Serial.print(", Humidade:");    Serial.println(humidity);
      }
      delay(1000);
      client.stop();
    }
  } else {
     //Serial.print("No New Message");
  }
  //delay(15000);
}
