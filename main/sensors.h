#ifndef SENSORS_H
#define SENSORS_H

#include <string.h>
#include <arduino.h>
#include <SoftwareSerial.h>
#include <Dps310.h>
#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h"


void get_sensordata(Adafruit_SHT31 sht31, Dps310 Dps310PressureSensor);

#endif
