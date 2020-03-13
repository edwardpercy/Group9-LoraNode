#ifndef SERIAL_H
#define SERIAL_H

#include <Time.h>
#include <TimeLib.h>
#include <string.h>
#include <arduino.h>
#include "sdcard.h"
#include "receive.h"
#include "transmit.h"

extern bool show_debug;
extern bool master_node;

void debug(String message);
void gui_send(String type, String message);
int gui_receive();

#endif
