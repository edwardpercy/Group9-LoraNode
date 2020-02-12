#ifndef TRANSMIT_H
#define TRANSMIT_H

#include <Time.h>
#include <TimeLib.h>
#include <string.h>
#include <arduino.h>
#include <SoftwareSerial.h>

extern int id;
extern bool ms_initiator;
int Transmit_String(String input,SoftwareSerial &loraSerial);
int Transmit_Hex(String input,SoftwareSerial &loraSerial);
int Transmit_LastSync(SoftwareSerial &loraSerial);

#endif
