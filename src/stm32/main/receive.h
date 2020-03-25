#ifndef RECEIVE_H
#define RECEIVE_H

#include <Time.h>
#include <TimeLib.h>
#include <arduino.h>
#include "serial.h"
#include "transmit.h"

extern HardwareSerial loraSerial; //is this required?
extern int currentTurnID;
extern bool confirmation;
extern bool master_node;

int MasterReceiver();
int Wait_For_Confirm();
int Receive_String(bool Synced);
String ProcessMessage(bool Synced,String str);
char h2c(char c1, char c2);
void SyncTime(String ReceivedLastSync);


#endif
