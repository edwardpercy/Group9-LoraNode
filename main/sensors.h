#ifndef SENSORS_H
#define SENSORS_H

#include <string.h>
#include <arduino.h>
#include <Dps310.h>
#include "Adafruit_SHT31.h"

extern Dps310 Dps310PressureSensor;
extern Adafruit_SHT31 sht31;

//void get_sensordata(Adafruit_SHT31 sht31, Dps310 Dps310PressureSensor);
float get_sensordata();

#endif
