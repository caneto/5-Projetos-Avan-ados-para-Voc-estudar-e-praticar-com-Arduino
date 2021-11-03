#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
 
RF24 radio(PB0, PA4); // CE, CSN on Blue Pill
const uint64_t address = 0xF0F0F0F0E1LL;
 
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
   Serial.begin(9600);
   radio.begin();
   Serial.print("ADDRESS :");
   radio.openReadingPipe(0, address);   //Definir o endereço no qual receberemos os dados
   radio.setPALevel(RF24_PA_MIN);       
   radio.startListening();              //Isso define o módulo como receptor
}
 
int recvData()
{
  if ( radio.available() ) 
  {
    radio.read(&data, sizeof(MyData));
    return 1;
  }
  return 0;
} 

void loop()
{
  if(recvData())
  {
    Serial.println("Data Received:");
    Serial.print("Packet No. = ");
    Serial.println(data.counter);
    Serial.print("Temperatura = ");
    Serial.print(data.temperature);
    Serial.println("*C");
    Serial.print("Pressao = ");
    Serial.print(data.pressure);
    Serial.println("hPa");
    Serial.print("Altitude Aprox. = ");
    Serial.print(data.altitude);
    Serial.println("m");
    Serial.print("Humidade = ");
    Serial.print(data.humidity);
    Serial.println("%");
    Serial.println();
  }
}
