#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
 
RF24 radio(PB0, PA4); // CE, CSN no Blue Pill
const uint64_t address = 0xF0F0F0F0E1LL;
boolean button_state = 0;
 
void setup() 
{
   Serial.begin(9600);
   radio.begin();
   Serial.print("ADDRESS :");
   radio.openReadingPipe(0, address);  //Definir o endereço no qual receberemos os dados
    //Você pode definir isso como mínimo ou máximo, dependendo da distância entre o transmissor e o receptor.  
   radio.setPALevel(RF24_PA_MIN);      
   radio.startListening();              //Isso define o módulo como receptor
}

void loop()
{
   if (radio.available())                          //Procurando os dados.
   {
     Serial.println("Radio is sniffing");
     char text[32] = "";                           //Salvando os dados de entrada
     radio.read(&text, sizeof(text));    //Lendo os dados
     Serial.println(text);
   }
}
