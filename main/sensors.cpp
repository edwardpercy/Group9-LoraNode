#include "sensors.h"

//float get_sensordata(){
//
//  float t = sht31.readTemperature();
//  float h = sht31.readHumidity();
//
//  Serial.println(t);
//  Serial.println(h);
//  return t;
//}
float get_sensordata(){
  float pressure;
  int t = sht31.readTemperature();
  int h = sht31.readHumidity();

  Dps310PressureSensor.measurePressureOnce(pressure);
 
  Serial.println("Pressure: " + String(pressure));
  Serial.println("Temp: " + String(t));
  Serial.println("Humidity: " + String(h));
  return pressure;
}
