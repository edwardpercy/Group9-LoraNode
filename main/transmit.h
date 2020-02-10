#ifndef TRANSMIT_H
#define TRANSMIT_H

#include <Time.h>
#include <TimeLib.h>
#include <string.h>
#include <arduino.h>
#include <SoftwareSerial.h>

extern int id;

int Transmit_String(String input,SoftwareSerial &loraSerial);
int Transmit_Hex(String input,SoftwareSerial &loraSerial);
int Transmit_LastSync(SoftwareSerial &loraSerial);

#endif
