#include <Time.h>
#include <TimeLib.h>
#include <SoftwareSerial.h>
#include <stdio.h>
#include <string.h>
#include "transmit.h"
#include "utilities.h"
#include "receive.h"
//#include "sensors.h"
#include "sdcard.h"
#include <SPI.h>
#include <SD.h>

HardwareSerial loraSerial(PB7,PB6); // RX, TX

bool Synced = false;
const PROGMEM int SyncFreq = 40;
const PROGMEM int MasterSyncFreq = 1000;
const PROGMEM int SyncRetries = 14;
int LastSync = 0;
int Sync = 0;
int SyncAttempt = 0;
int id = 0;
bool ms_initiator = true; 
bool ms_finish = true;

//Adafruit_SHT31 sht31;
//Dps310 Dps310PressureSensor;
 
void setup() {
  Serial.begin(57600);
  loraSerial.begin(9600);
  loraSerial.setTimeout(1000);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }




  int Startup_Check = 0;
 
  lora_autobaud();


  loraSerial.readStringUntil('\n');
  loraSerial.println(F("sys get ver"));
  Serial.println(loraSerial.readStringUntil('\n'));
  loraSerial.println(F("mac pause"));
  Serial.println(loraSerial.readStringUntil('\n'));

  Serial.println(F("NODE: Starting"));
  loraSerial.println(F("radio set mod lora"));
  Startup_Check += wait_for_ok();
  loraSerial.println(F("radio set freq 869100000"));
  Startup_Check += wait_for_ok();
  loraSerial.println(F("radio set wdt 2000")); //disable for continuous reception (Currently: 2s)
  Startup_Check += wait_for_ok();
  loraSerial.println(F("radio set sync 18"));
  Startup_Check += wait_for_ok();

  //Sensors setup
  //Dps310PressureSensor = Dps310();
  //sht31 = Adafruit_SHT31();
  //Dps310PressureSensor.begin(Wire);
  //sht31.begin(0x44);
  logs("a");
  //get_sensordata(sht31, Dps310PressureSensor);
  //get_sensordata();
  
  if (Startup_Check > 0){
    Serial.println(F("NODE: F"));
    delay(20000);
  }
  else{
    Serial.println(F("NODE: S"));
    
  }
  delay(5000);
 

}

void loop() {
    time_t TimeNow = now();
    LastSync = TimeNow%MasterSyncFreq;
    Sync = TimeNow%SyncFreq;
    Serial.println(LastSync);
    
    if (Synced == false) {
  
      StartupSync(); //Ran when NODE is started to sync time with nearby nodes
    }
    else if (LastSync <= 500 && ms_finish == false){ //Runs for a maximum of 500s or until the sync is complete (Master sync)
      
      SendReceiveLoop();
    }
    else if (LastSync <= 5){ //Indicates the start of the master sync
     
      ms_finish = false;
    }
    
    
    else if (Sync <= 10){ //Minor Sync
      Serial.println("ID: " + String(id) + " TIME:" + String(LastSync) + " Init: " + String(ms_initiator));
      SendReceiveLoop();
    }
      
    else if (Synced == true) { //Sleep - low power mode
      int t = now();
      Serial.println(String(t));
      loraSerial.println(F("sys sleep 29"));
      Serial.println(wait_for_ok());

      Serial.println(F("SLEEP"));
      delay(30150); //Ran once every 2 seconds (Radio listening time)
    }
    else delay(1000);

}

void SendReceiveLoop() {
  int out;
  if ((out = Receive_String(Synced)) == 0){
    Serial.println(F("rx L")); //Listening
  }
  else if (out  == 2){
    Serial.println(F("rx D")); //Data
  }
  else if (out  == 3){
    Serial.println(F("rx C")); //Confirmation
  }
  else{
    Serial.println(F("rx E")); //Error
  }

//  if (random(0,2)== 0){
//      temp = get_sensordata(sht31);
//      if (Transmit_String("*C" + String(id) + String(temp),loraSerial) == 0) Serial.println("tx S");
//      else Serial.println(F("tx E"));
//  }
}

void StartupSync() {
  int out;
  Serial.println(F("NODE: SA"));
  Transmit_Hex(F("3C3C"));
  
  if ((out = Receive_String(Synced)) == 0);
  else if (out  == 2) Serial.println(F("rx D"));
  else if (out  == 3) Serial.println(F("rx C"));
  else if (out  == 4) {
    Synced = true;
  }
  else Serial.println(F("rx E"));
  SyncAttempt += 1;
  if (SyncAttempt > SyncRetries) {
    Synced = true;

 
  }
  
}
