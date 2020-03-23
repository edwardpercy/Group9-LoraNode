#ifndef SDCARD_H
#define SDCARD_H

#include <string.h>
#include <arduino.h>
#include <Time.h>
#include <TimeLib.h>
#include <SPI.h>
#include <SD.h>
#include "serial.h"

void logs(String info);
int SDCheck();
int readSD();
#endif
