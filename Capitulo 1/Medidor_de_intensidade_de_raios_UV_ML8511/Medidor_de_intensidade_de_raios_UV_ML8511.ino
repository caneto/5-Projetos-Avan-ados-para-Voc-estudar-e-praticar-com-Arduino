#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
// Definições de pinos de hardware
int UVOUT = A0; // Saída do sensor
int REF_3V3 = A1; // Energia de 3,3V na placa Arduino
 
void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
 
  pinMode(UVOUT, INPUT);
  pinMode(REF_3V3, INPUT);
 
  Serial.println("Exemplo ML8511");
}
 
void loop()
{
  int uvLevel = averageAnalogRead(UVOUT);
  int refLevel = averageAnalogRead(REF_3V3);
  
  // Use o pino de alimentação de 3,3V como referência para obter um valor de saída muito preciso do sensor
  float outputVoltage = 3.3 / refLevel * uvLevel;
  
  // Converta a voltagem para um nível de intensidade UV
  float uvIntensity = mapfloat(outputVoltage, 0.99, 2.8, 0.0, 15.0); 
 
  Serial.print("resultado: ");
  Serial.print(refLevel);
  Serial.print("Saída ML8511: ");
  Serial.print(uvLevel);
  Serial.print(" / Tensão ML8511: ");
  Serial.print(outputVoltage);
  Serial.print(" / Intensidade UV (mW/cm^2): ");
  Serial.print(uvIntensity);
  
  lcd.clear();
  lcd.print("Intensidade de raios UV");
  lcd.setCursor(0, 1);
  lcd.print(uvIntensity);
  lcd.print(" mW/cm^2");
  
  Serial.println();
  
  delay(200);
}
 
// Faz uma média de leituras em um determinado pino
// Retorna a média
int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0; 
  for(int x = 0 ; x < numberOfReadings ; x++)   runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;
  return(runningValue);
}
 
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
