#ifndef SYNC_H
#define SYNC_H

#include <Time.h>
#include <TimeLib.h>
#include <string.h>
#include <arduino.h>
#include "sdcard.h"
#include "receive.h"
#include "transmit.h"

extern String LatestReading;
extern bool ms_initiator;
extern bool sync_active;
void relay_master_signal();
void master_sync();
int slaveReceiver();

#endif
