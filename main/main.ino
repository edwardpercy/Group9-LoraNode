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
int SyncFreq = 20;
int MasterSyncFreq = 1000;
int SyncRetries = 10;
int LastSync = 0;
int Sync = 0;
int SyncAttempt = 0;
int temp = 0;

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
  //loraSerial.println("radio set pwr 14");
  //Startup_Check += wait_for_ok(loraSerial);
  //loraSerial.println("radio set sf sf7");
  //Startup_Check += wait_for_ok(loraSerial);
  //loraSerial.println("radio set afcbw 41.7");
  //Startup_Check += wait_for_ok(loraSerial);
  //loraSerial.println("radio set rxbw 125");
  //  Startup_Check += wait_for_ok(loraSerial);
  //  loraSerial.println("radio set prlen 8");
  //  Startup_Check += wait_for_ok(loraSerial);
  //  loraSerial.println("radio set crc on");
  //  Startup_Check += wait_for_ok(loraSerial);
  //  loraSerial.println("radio set iqi off");
  //  Startup_Check += wait_for_ok(loraSerial);
  //  loraSerial.println("radio set cr 4/5");
  //  Startup_Check += wait_for_ok(loraSerial);
  loraSerial.println("radio set wdt 2000"); //disable for continuous reception (Currently: 2s)
  Startup_Check += wait_for_ok(loraSerial);
  loraSerial.println("radio set sync 12");
  Startup_Check += wait_for_ok(loraSerial);
  //loraSerial.println("radio set bw 125");
  //Startup_Check += wait_for_ok(loraSerial);

  //Sensors setup
  //Dps310 Dps310PressureSensor = Dps310();
  Adafruit_SHT31 sht31 = Adafruit_SHT31();
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
    Serial.println(LastSync);

    
    if (Synced == false && TimeNow > 3) StartupSync(); //Ran when NODE is started
    
    else if (LastSync <= 30){ //Runs for 30 seconds every major sync
      logs("MS");
      SyncAttempt = 0; 
      delay(2000);
    }
    else if (Sync <= 5){ //Runs for 5 seconds every minor sync
      
      SendReceiveLoop();
    }
      
    else if (Synced == true) delay(2000); //Ran once every 2 seconds (Radio listening time)
    else delay(1000);

}

void SendReceiveLoop() {
//
//  if (Serial.available() > 0) { //Read from serial monitor and send over UART LoRa wireless module
//      String input = Serial.readStringUntil('\n');
//      if (Transmit_String(input,loraSerial) == 0) Serial.println("tx S");
//      else Serial.println(F("tx E"));
//  }

  if ((out = Receive_String(Synced,loraSerial)) == 0){
    Serial.println(("rx L"));
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

  if (random(0,2)== 0){
      if (Transmit_String(String(temp),loraSerial) == 0) Serial.println("tx S");
      else Serial.println(F("tx E"));
  }
}

void StartupSync() {
  logs("SA"); //Sync Attempt
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
