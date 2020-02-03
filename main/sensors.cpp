#include "sensors.h"

void get_sensordata(Adafruit_SHT31 sht31,Dps310 Dps310PressureSensor){
  float pressure;
  float ret;
  float t = sht31.readTemperature();
  float h = sht31.readHumidity();

  ret = Dps310PressureSensor.measurePressureOnce(pressure);
  if (ret != 0)
  {
    //Something went wrong.
    //Look at the library code for more information about return codes
    Serial.print("FAIL! ret = ");
    Serial.println(ret);
  }
  else
  {
    Serial.print("Pressure: ");
    Serial.print(pressure);
    Serial.println(" Pascal");
  }

  if (! isnan(t)) {  // check if 'is not a number'
    Serial.print("Temp *C = "); Serial.println(t);
  } else {
    Serial.println("Failed to read temperature");
  }

  if (! isnan(h)) {  // check if 'is not a number'
    Serial.print("Hum. % = "); Serial.println(h);
  } else {
    Serial.println("Failed to read humidity");
  }
  Serial.println();
}
