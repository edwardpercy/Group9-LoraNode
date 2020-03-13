#ifndef SERIAL_H
#define SERIAL_H

#include <string.h>
#include <arduino.h>
#include "sdcard.h"

extern bool show_debug;
extern bool master_node;

void debug(String message);
void gui_send(String type, String message);
int gui_receive();

#endif
