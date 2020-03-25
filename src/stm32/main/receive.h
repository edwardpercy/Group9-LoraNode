#ifndef RECEIVE_H
#define RECEIVE_H

#include <Time.h>
#include <TimeLib.h>
#include <arduino.h>
#include "serial.h"
#include "transmit.h"
#include "sync.h"

extern HardwareSerial loraSerial; //is this required?
extern int currentTurnID;
extern bool confirmation;
extern bool master_node;
extern bool sync_active;
extern int id;

int MasterReceiver();
int Wait_For_Confirm();
int Receive_String(bool Synced);
String ProcessMessage(bool Synced,String str);
char h2c(char c1, char c2);
void SyncTime(String ReceivedLastSync);
void receive_readings(String str);

#endif
