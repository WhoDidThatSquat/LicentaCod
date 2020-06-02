#include <Wire.h>

int pin = A0;
float outTemperatura = 0;
int oldV;
int tempPinSend = 7;
int tempPinEN = 6;

void setup() {
  Serial.begin(115200); 
}


void loop() {
  float temperatura = readTempInCelsius(20,pin);
  int vProcent;
  int valoareIluminare = analogRead(A1);
  int iluminareProcent = valoareIluminare / 10;
  //Serial.print("lumina: ");
  //Serial.println(valoareIluminare, DEC); 

  int v = analogRead(A2);
  int umiditate = (1023 - v)/10;
//  v = smooth(v, 0.99, oldV);
//  oldV = v;
 // v=v/10;
  //Serial.print("Umiditate: ");
  //Serial.println(v);
  //Serial.write(temperatura,5);
  Serial.print(temperatura);
  Serial.print(" ");
  Serial.print(umiditate);
  Serial.print(" ");
  Serial.print(iluminareProcent);
  Serial.println();

  delay(3000);
  Serial.flush();
  
}
float readTempInCelsius(int count, int pin) {
  float temperaturaMediata = 0;
  float sumaTemperatura = 0;
  for (int i =0; i < count; i++) {
    int reading = analogRead(pin);
    float voltage = reading * 5.0;
    voltage /= 1024.0;
    outTemperatura = voltage;
    float temperatureCelsius = (voltage - 0.5) * 100 ;
    sumaTemperatura = sumaTemperatura + temperatureCelsius;
  }
  return sumaTemperatura / (float)count;
}

//int smooth(int data, float filterVal, float smoothedVal) {
//  if (filterVal > 1){      
//    filterVal = .99;
//  }
//  else if (filterVal <= 0)
//    filterVal = 0;
//  smoothedVal = (data * (1 - filterVal)) + (smoothedVal  *  filterVal);
//  return (int)smoothedVal;
//}
