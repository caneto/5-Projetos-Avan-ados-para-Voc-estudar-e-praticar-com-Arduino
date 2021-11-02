#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
 
int UVOUT = A0; //Saída do sensor
int REF_3V3 = A1; //Alimentação de 3,3V na placa Arduino
 
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

void setup()
{
  Serial.begin(9600);
  pinMode(UVOUT, INPUT);
  pinMode(REF_3V3, INPUT);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //inicializar com o addr I2C 0x3C (128x64)
  display.clearDisplay();
  Serial.println("ML8511 example");
}
 
void loop()
{
  int uvLevel = averageAnalogRead(UVOUT);
  int refLevel = averageAnalogRead(REF_3V3);
  
  //Use o pino de alimentação de 3,3 V como referência para obter um valor de saída muito preciso do sensor
  float outputVoltage = 3.3 / refLevel * uvLevel;
  //Converta a voltagem para um nível de intensidade de UV
  float uvIntensity = mapfloat(outputVoltage, 0.99, 2.8, 0.0, 15.0); 
 
  Serial.print("resultado: ");
  Serial.print(refLevel);
  Serial.print("Saída ML8511: ");
  Serial.print(uvLevel);
  Serial.print(" / Tensão ML8511: ");
  Serial.print(outputVoltage);
 
  Serial.print(" / Intensidade UV (mW/cm^2): ");
  Serial.print(uvIntensity);
  Serial.println();
 
  display.setCursor(20,0);  //você está em exibição
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Intensidade de raios UV");
  display.setCursor(20,20);  //você está em exibição
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.println(uvIntensity);
  display.setCursor(20,45);  //você está em exibição
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.println("mW/cm^2");
  display.display();
 
  delay(300);
  display.clearDisplay();
}
 
// Faz uma média de leituras em um determinado pino
// Retorna a média
int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0; 
  for(int x = 0 ; x < numberOfReadings ; x++) runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;
  return(runningValue);
}
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
