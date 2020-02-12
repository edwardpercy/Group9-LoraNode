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

SoftwareSerial loraSerial(8,9);
int out;
bool Synced = false;
int SyncFreq = 40;
int MasterSyncFreq = 1000;
int SyncRetries = 20;
int LastSync = 0;
int Sync = 0;
int SyncAttempt = 0;
float temp = 0;
int id = 0;
bool ms_initiator = true; 
bool ms_finish = true;

Adafruit_SHT31 sht31;

void setup() {
  
  int Startup_Check = 0;
  String str;

  pinMode(13, OUTPUT);


  Serial.begin(57600);

  pinMode (4, OUTPUT);
  
  // Reset rn2483
  pinMode(7, OUTPUT);
//  digitalWrite(7, HIGH);
//  digitalWrite(7, LOW);
//  delay(500);
  digitalWrite(7, HIGH);

  loraSerial.begin(9600);
  
  loraSerial.setTimeout(1000);
  lora_autobaud(loraSerial);


  loraSerial.listen();
  str = loraSerial.readStringUntil('\n');
  loraSerial.println("sys get ver");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  loraSerial.println("mac pause");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);

  Serial.println("NODE: Starting");
  loraSerial.println("radio set mod lora");
  Startup_Check += wait_for_ok(loraSerial);
  loraSerial.println("radio set freq 869100000");
  Startup_Check += wait_for_ok(loraSerial);
  loraSerial.println("radio set wdt 2000"); //disable for continuous reception (Currently: 2s)
  Startup_Check += wait_for_ok(loraSerial);
  loraSerial.println("radio set sync 18");
  Startup_Check += wait_for_ok(loraSerial);

  //Sensors setup
  //Dps310 Dps310PressureSensor = Dps310();
  sht31 = Adafruit_SHT31();
  //Dps310PressureSensor.begin(Wire);
  sht31.begin(0x44);

  load_sd();

  //get_sensordata(sht31, Dps310PressureSensor);
  temp = get_sensordata(sht31);
  
  if (Startup_Check > 0){
    Serial.println(F("NODE: F"));

    logs("BF");
  }
  else{
    Serial.println(F("NODE: S"));

    logs("BS");
  }

}

void loop() {

    

    time_t TimeNow = now();
    LastSync = TimeNow%MasterSyncFreq;
    Sync = TimeNow%SyncFreq;
    
    
    
    if (Synced == false) {
      StartupSync(); //Ran when NODE is started to sync time with nearby nodes
    }
    else if (LastSync <= 2){ //Indicates the start of the master sync
      ms_finish = false;
    }
    else if (LastSync <= 500 && ms_finish == false){ //Runs for a maximum of 500s or until the sync is complete (Master sync)
      Serial.println(("MS" + String(ms_finish)));
      SendReceiveLoop();
    }
    else if (Sync <= 10){ //Minor Sync
      Serial.println("ID: " + String(id) + " TIME:" + String(LastSync) + " Init: " + String(ms_initiator));
      SendReceiveLoop();
    }
      
    else if (Synced == true) { //Sleep - low power mode
      Serial.println(String(now()));
      loraSerial.println("sys sleep 29");
      Serial.println(wait_for_ok(loraSerial));

      Serial.println(F("SLEEP"));
      delay(30100); //Ran once every 2 seconds (Radio listening time)
    }
    else delay(1000);

}

void SendReceiveLoop() {

  if ((out = Receive_String(Synced,loraSerial)) == 0){
    Serial.println(("rx L")); //Listening
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
  //logs("SA"); //Sync Attempt
  Serial.println(F("NODE: SA"));
  Transmit_Hex("3C3C",loraSerial);
  
  if ((out = Receive_String(Synced,loraSerial)) == 0);
  else if (out  == 2) Serial.println(F("rx D"));
  else if (out  == 3) Serial.println(F("rx C"));
  else if (out  == 4) {
    Synced = true;
    logs("NNS"); //Nearby node sync
  }
  else Serial.println(F("rx E"));
  SyncAttempt += 1;
  if (SyncAttempt > SyncRetries) {
    Synced = true;
    logs("AS"); //Automatic Sync
    
  }
  
}
