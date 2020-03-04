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
bool ms_initiator = true;

const PROGMEM int MasterSyncFreq = 1440;
const PROGMEM int TimeSyncFreq = 240;
const PROGMEM int LocalSyncFreq = 120;
const PROGMEM int ListenFreq = 20;

const PROGMEM int MasterSyncPeriod = 100;
const PROGMEM int TimeSyncPeriod = 10;
const PROGMEM int ListenPeriod = 4;

const PROGMEM int SyncRetries = 11;

int SyncAttempt = 0;
int id = 0;


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


  //reset rn2483
  pinMode(PA11, OUTPUT);
  digitalWrite(PA11, LOW);
  delay(500);
  digitalWrite(PA11, HIGH);

  delay(100); //wait for the RN2xx3's startup message
  loraSerial.flush();

  
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
    
  }
  else{
    Serial.println(F("NODE: Startup Success"));
    logs("Startup Success");
  }
  delay(5000);
 

}

void loop() {

    //Time Keeping
    time_t TimeNow = now();
    int Time = TimeNow;

    //Startup Sync - Ran on startup to sync time with nearby nodes
    if (Synced == false) {
      Serial.println("Startup Sync - ID: " + String(id) + " TIME:" + String(Time) + " Init: " + String(ms_initiator));
      StartupSync(); //Ran when NODE is started to sync time with nearby nodes
    }


    //Master Sync - Sync with the master node 
    else if (TimeNow%MasterSyncFreq <= MasterSyncPeriod && TimeNow >MasterSyncPeriod){ //Run for 100s every 1440s
      Serial.println("Master Sync - ID: " + String(id) + " TIME:" + String(Time) + " Init: " + String(ms_initiator));
      ReceiveLoop();
    }


    //Time Sync - Sync time with master node
    else if (TimeNow%TimeSyncFreq <= TimeSyncPeriod && TimeNow > TimeSyncPeriod){ //Run for 10s every 240s
      Serial.println("Time Sync - ID: " + String(id) + " TIME:" + String(Time) + " Init: " + String(ms_initiator));
      Transmit_Hex(F("3C3C"));
    }


    //Local Sync - Take sensor readings and send them to nearby nodes
    else if (TimeNow%LocalSyncFreq <= 4){ 
      Serial.println("Local Sync - ID: " + String(id) + " TIME:" + String(Time) + " Init: " + String(ms_initiator));

      //Read and save sensor data to sd-card *TESTING*
      float *data = get_sensordata();
      Serial.println("DATA P(" + String(data[0]) + ") T(" + String(data[1]) + ") H(" + String(data[2])+ ")");
      logs("DATA P(" + String(data[0]) + ") T(" + String(data[1]) + ") H(" + String(data[2])+ ")");

      if (random(0,2)== 0){
          if (Transmit_String("*C" + String(id) + String(data[1])) == 0) Serial.println("tx S");
          else Serial.println(F("tx E"));
      }
      else{
        ReceiveLoop();
      }


    }


    //Listen Period - Listen for incoming messages
    else if (TimeNow%ListenFreq <= ListenPeriod){ 
      Serial.println("Listening - ID: " + String(id) + " TIME:" + String(Time) + " Init: " + String(ms_initiator));
      ReceiveLoop();
    }


    //Sleep - low power mode
    else if (Synced == true) {
      loraSerial.println(F("sys sleep 15")); //Put LoRa to sleep
      Serial.println(wait_for_ok());

      Serial.println(F("SLEEP"));
      delay(16050); 
    }
    else delay(1000);

}

void ReceiveLoop() {
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
