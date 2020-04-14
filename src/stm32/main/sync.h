#ifndef SYNC_H
#define SYNC_H

#include <Time.h>
#include <TimeLib.h>
#include <string.h>
#include <arduino.h>
#include "sdcard.h"
#include "receive.h"
#include "transmit.h"
#include "sensors.h"
#include "utilities.h"
#include <queue>

extern String LatestReading;
extern bool ms_initiator;
extern bool sync_active;
extern int id;
void relay_master_signal(int timeout_val);
void master_sync();
int slaveReceiver();

#endif
