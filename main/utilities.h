#ifndef UTILITIES_H
#define UTILITIES_H

#include <string.h>
#include <arduino.h>
extern HardwareSerial loraSerial;
void lora_autobaud();
int wait_for_ok_plus_reset();
int wait_for_ok();


#endif
