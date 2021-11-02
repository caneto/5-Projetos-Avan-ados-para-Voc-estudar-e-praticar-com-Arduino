#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <nRF24L01.h>
#include <RF24.h>
 
RF24 radio(PB0, PA4); // CE, CSN on Blue Pill 
const uint64_t address = 0xF0F0F0F0E1LL;
int counter = 0;
 
float temperature;
float humidity;
float altitude;
float pressure;
 
#define SEALEVELPRESSURE_HPA (1013.25)
 
Adafruit_BME280 bme;
 
struct MyData 
{
  int counter;
  float temperature;
  float humidity;
  float altitude;
  float pressure;
};
MyData data;

void setup() 
{
   Serial.begin(115200);
   radio.begin();                  // Iniciando a comunicação sem fio
   radio.openWritingPipe(address); // Definir o endereço para onde enviaremos os dados
   radio.setPALevel(RF24_PA_MIN);  // Você pode definir como mínimo ou máximo, dependendo da distância entre o transmissor e o receptor.
   radio.stopListening();          // Isso define o módulo como transmissor 
   if (!bme.begin(0x76)) 
   {
      Serial.println("Não foi possível encontrar um sensor BME280 válido, verifique a ligação!");
      while (1);
   }
}
void loop()
{
  data.counter = counter;
  data.temperature = bme.readTemperature();
  data.pressure = bme.readPressure() / 100.0F;
  data.altitude = bme.readAltitude(SEALEVELPRESSURE_HPA);
  data.humidity = bme.readHumidity();
  
  Serial.print("Packet No. = ");
  Serial.println(data.counter);
  Serial.print("Temperatura = ");
  Serial.print(data.temperature);
  Serial.println("*C");
 
  Serial.print("Pressão = ");
  Serial.print(data.pressure);
  Serial.println("hPa");
 
  Serial.print("Aprox. Altitude = ");
  Serial.print(data.altitude);
  Serial.println("m");
 
  Serial.print("Humidade = ");
  Serial.print(data.humidity);
  Serial.println("%");
 
  Serial.println();
  
  radio.write(&data, sizeof(MyData));
  
  Serial.println("Data Packet Sent");
  Serial.println("");
  
  counter++;
  delay(5000);
}
