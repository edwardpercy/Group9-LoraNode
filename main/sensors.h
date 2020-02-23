#ifndef SENSORS_H
#define SENSORS_H

#include <string.h>
#include <arduino.h>
#include <Dps310.h>
#include "Adafruit_SHT31.h"

extern Dps310 Dps310PressureSensor;
extern Adafruit_SHT31 sht31;


float get_sensordata();

#endif
