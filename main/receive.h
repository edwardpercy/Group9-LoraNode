#ifndef RECEIVE_H
#define RECEIVE_H

#include <Time.h>
#include <TimeLib.h>
#include <arduino.h>
#include <SoftwareSerial.h>
#include "transmit.h"

extern SoftwareSerial loraSerial;

int Receive_String(bool Synced);
String ProcessMessage(bool Synced,String str);
char h2c(char c1, char c2);
void SyncTime(String ReceivedLastSync);


#endif
