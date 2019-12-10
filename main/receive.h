#ifndef RECEIVE_H
#define RECEIVE_H

#include <arduino.h>
#include <SoftwareSerial.h>
#include "transmit.h"

int Receive_String(SoftwareSerial &loraSerial);
String ProcessMessage(String str);
char h2c(char c1, char c2);

#endif
