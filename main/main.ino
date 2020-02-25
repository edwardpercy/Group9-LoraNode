#include <Time.h>
#include <TimeLib.h>
#include <SoftwareSerial.h>
#include <stdio.h>
#include <string.h>
#include "transmit.h"
#include "utilities.h"
#include "receive.h"
#include "sensors.h"
#include "sdcard.h"
#include <SPI.h>
#include <SD.h>
#include <Dps310.h>
#include "Adafruit_SHT31.h"

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

Adafruit_SHT31 sht31;
Dps310 Dps310PressureSensor;
 
void setup() {
  Serial.println(F("NODE: Initiating"));
  delay(3000);

  //Setup Serial Connections
  Serial.begin(57600); //USB
  loraSerial.begin(9600); //LoRa
  loraSerial.setTimeout(1000);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  //Setup Pressure/Humidity/Temp Sensors (I2C Connection)
  Wire.setSDA(PA10); //I2C Wire Setup
  Wire.setSCL(PA9);
  Wire.begin();
  Dps310PressureSensor = Dps310();
  sht31 = Adafruit_SHT31();
  Dps310PressureSensor.begin(Wire, 0x77);
  sht31.begin(0x44);
 
  //Setup LoRa RN2483 (Serial Connection)
  lora_autobaud();
  loraSerial.readStringUntil('\n');
  loraSerial.println(F("sys get ver"));
  Serial.println(loraSerial.readStringUntil('\n'));
  loraSerial.println(F("mac pause")); //To allow Transceiving using LoRa protocol the mac must be paused
  Serial.println(loraSerial.readStringUntil('\n'));
  
  //Set RN2483 Parameters
  int Startup_Check = 0;
  loraSerial.println(F("radio set mod lora")); //Set the RN2483 to use LoRa modulation
  Startup_Check += wait_for_ok();
  loraSerial.println(F("radio set freq 869100000")); //869MHz Frequency
  Startup_Check += wait_for_ok();
  loraSerial.println(F("radio set wdt 2000")); //Watchdog Timer set to 2s to allow the RN2483 to listen for 2s 
  Startup_Check += wait_for_ok();
  loraSerial.println(F("radio set sync 18")); //Sync word used to that other people's LoRa modules don't interfere
  Startup_Check += wait_for_ok();

  //SD-Card Check
  Startup_Check += SDCheck();
  
  if (Startup_Check > 0){
    Serial.println(F("NODE: Startup Failed"));
    logs("Startup Fail");
    while(1){
    }
  }
  else{
    Serial.println(F("NODE: Startup Success"));
    logs("Startup Success");
  }
  delay(5000);
 

}

void loop() {

    //Read and save sensor data to sd-card *TESTING*
    float *data = get_sensordata();
    Serial.println("DATA P(" + String(data[0]) + ") T(" + String(data[1]) + ") H(" + String(data[2])+ ")");
    logs("DATA P(" + String(data[0]) + ") T(" + String(data[1]) + ") H(" + String(data[2])+ ")");

    //Time Keeping
    time_t TimeNow = now();
    LastSync = TimeNow%MasterSyncFreq;
    Sync = TimeNow%SyncFreq;
    Serial.println(LastSync);
    
    if (Synced == false) {
  
      StartupSync(); //Ran when NODE is started to sync time with nearby nodes
    }

    //WIP WIP WIP - Major Sync
    else if (LastSync <= 500 && ms_finish == false){ //Runs for a maximum of 500s or until the sync is complete (Master sync)
      
      SendReceiveLoop();
    }
    else if (LastSync <= 5){ //Indicates the start of the master sync
     
      ms_finish = false;
    }
    //WIP WIP WIP
    
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
 // Transmit_String("TEst");
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
