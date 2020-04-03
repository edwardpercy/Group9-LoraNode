#ifndef TRANSMIT_H
#define TRANSMIT_H

#include <Time.h>
#include <TimeLib.h>
#include <string.h>
#include <arduino.h>


extern HardwareSerial loraSerial;
extern int id;
extern int currentTurnID;
extern bool ms_initiator;
extern bool confirmation;
extern String LastTransmitMsg;
extern const PROGMEM int MasterSyncFreq;

int Transmit_String(String input_c);
int Transmit_Hex(String input);
int Transmit_LastSync();

#endif
