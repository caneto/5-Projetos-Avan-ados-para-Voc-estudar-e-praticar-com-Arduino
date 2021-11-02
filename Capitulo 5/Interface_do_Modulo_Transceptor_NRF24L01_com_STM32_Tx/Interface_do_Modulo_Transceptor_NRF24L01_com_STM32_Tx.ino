#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
 
RF24 radio(PB0, PA4); // CE, CSN no Blue Pill 
const uint64_t address = 0xF0F0F0F0E1LL;
int counter = 0;
 
void setup() 
{
  Serial.begin(9600);
   //Iniciando a comunicação sem fio
   radio.begin();       //Definir o endereço para onde enviaremos os dados
   radio.openWritingPipe(address); 
   //Você pode definir como mínimo ou máximo, dependendo da distância entre o transmissor e o receptor.
   radio.setPALevel(RF24_PA_MIN);   //Isso define o módulo como transmissor
   radio.stopListening();
}

void loop()
{
  char text[] = " Ola Mundo";
  char str[50];
  sprintf(str,"%s %d",text,counter);
  radio.write(&str, sizeof(str));  
 
  Serial.println(str);
  counter++;
  delay(2000);
}
