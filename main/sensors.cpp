#include "sensors.h"

float get_sensordata(Adafruit_SHT31 sht31){

  float t = sht31.readTemperature();
  float h = sht31.readHumidity();

  Serial.println(t);
  Serial.println(h);
  return t;
}
//void get_sensordata(Adafruit_SHT31 sht31,Dps310 Dps310PressureSensor){
//  float pressure;
//  int t = sht31.readTemperature();
//  int h = sht31.readHumidity();
//
//  Dps310PressureSensor.measurePressureOnce(pressure);
// 
//  Serial.println(pressure);
//  Serial.println(t);
//  Serial.println(h);
//
//}
