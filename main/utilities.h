#ifndef UTILITIES_H
#define UTILITIES_H

#include <string.h>
#include <arduino.h>
#include <SoftwareSerial.h>


void lora_autobaud(SoftwareSerial &loraSerial);
int wait_for_ok_plus_reset(SoftwareSerial &loraSerial);
int wait_for_ok(SoftwareSerial &loraSerial);
void toggle_led();
void led_on();
void led_off();

#endif
