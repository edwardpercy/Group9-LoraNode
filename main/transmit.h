#ifndef TRANSMIT_H
#define TRANSMIT_H

#include <Time.h>
#include <TimeLib.h>
#include <string.h>
#include <arduino.h>


extern HardwareSerial loraSerial;
extern int id;
extern bool ms_initiator;

int Transmit_String(String input);
int Transmit_Hex(String input);
int Transmit_LastSync();

#endif
