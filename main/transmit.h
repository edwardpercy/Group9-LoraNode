#ifndef TRANSMIT_H
#define TRANSMIT_H

#include <string.h>
#include <arduino.h>
#include <SoftwareSerial.h>


int Transmit_String(String input,SoftwareSerial &loraSerial);


#endif
