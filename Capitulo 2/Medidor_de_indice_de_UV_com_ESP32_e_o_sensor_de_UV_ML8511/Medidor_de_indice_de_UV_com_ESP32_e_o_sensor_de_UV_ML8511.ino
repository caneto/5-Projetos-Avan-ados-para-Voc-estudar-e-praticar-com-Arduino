#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
 
// Defina o endereço LCD para 0x27 para um display de 16 caracteres e 2 linhas
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
 
int UVOUT = 15; // Saída do sensor
int REF_3V3 = 4; //Alimentação de 3,3V na placa ESP32
 
void setup()
{
  // inicializa o LCD
  lcd.begin (16, 2);
  lcd.setBacklight(HIGH);
  pinMode(UVOUT, INPUT);
  pinMode(REF_3V3, INPUT);
}
 
// Faz uma média de leituras em um determinado pino
// Retorna a média
int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0; 
  for(int x = 0; x < numberOfReadings; x++)  runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;
  return(runningValue);
}
 
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
} 

void loop()
{
  int uvLevel = averageAnalogRead(UVOUT);
  int refLevel = averageAnalogRead(REF_3V3);
  
  // Use o pino de alimentação de 3,3 V como referência para obter um valor de saída muito preciso do sensor
  float outputVoltage = 3.3 / refLevel * uvLevel;
  
  float uvIntensity = mapfloat(outputVoltage, 0.99, 2.8, 0.0, 15.0); //Convert the voltage to a UV intensity level
 
  Serial.print("Saida: ");
  Serial.print(refLevel);
 
  Serial.print("Saída ML8511: ");
  Serial.print(uvLevel);
 
  Serial.print(" / ML8511 voltage: ");
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
