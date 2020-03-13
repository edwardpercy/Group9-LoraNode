#include "sensors.h"

float *get_sensordata(){
  float pressure;
  float t = sht31.readTemperature();
  float h = sht31.readHumidity();
  float *data = new float[3];
  
  Dps310PressureSensor.measurePressureOnce(pressure);

  data[0] = pressure;
  data[1] = t;
  data[2] = h;
  
  return data;
}
