#ifndef RECEIVE_H
#define RECEIVE_H

#include <Time.h>
#include <TimeLib.h>
#include <arduino.h>
#include <SoftwareSerial.h>
#include "transmit.h"


int Receive_String(SoftwareSerial &loraSerial);
String ProcessMessage(SoftwareSerial &loraSerial,String str);
char h2c(char c1, char c2);
void SyncTime(String ReceivedLastSync);


#endif
