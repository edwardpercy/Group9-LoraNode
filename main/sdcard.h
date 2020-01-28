#ifndef SDCARD_H
#define SDCARD_H

#include <string.h>
#include <arduino.h>

#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>

void load_sd();
void write_sd();
void logs(String info);
#endif
