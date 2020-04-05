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
#include "serial.h"
#include <SPI.h>
#include <SD.h>
#include <Dps310.h>
#include "Adafruit_SHT31.h"

HardwareSerial loraSerial(PB7,PB6); // RX, TX

bool Synced = false;
bool ms_initiator = true;
bool show_debug = true;
bool master_node = true;
bool confirmation = true;
bool sync_active = false;
int ResendRetries = 1;

String LatestReading;
String LastTransmitMsg = "";


const PROGMEM int ListenFreq = 20;
const PROGMEM int ListenPeriod = 10;
const PROGMEM int SyncRetries = 30;

int SyncAttempt = 0;
int id = 0;
int currentTurnID = 0;

Adafruit_SHT31 sht31;
Dps310 Dps310PressureSensor;


void setup() {
  debug("NODE: Initiating");
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
  
  if (master_node == true){
    loraSerial.println(F("radio set wdt 5000")); //Watchdog Timer set to 2s to allow the RN2483 to listen for 2s 
    Startup_Check += wait_for_ok();  
  }
  else{
    loraSerial.println(F("radio set wdt 2000")); //Watchdog Timer set to 2s to allow the RN2483 to listen for 2s 
    Startup_Check += wait_for_ok();  
  }
  
  loraSerial.println(F("radio set sync 18")); //Sync word used so that other people's LoRa modules don't interfere
  Startup_Check += wait_for_ok();

  //SD-Card Check
  //Startup_Check += SDCheck();
  SDCheck();
  
  if (Startup_Check > 0){
    debug("NODE: Startup Failed");
    logs("Startup Fail");
    while(1){
      gui_send("WARN", "Startup Fail"); //Keep-Alive
    }
  }
  else{
    debug("NODE: Startup Success");
    logs("Startup Success");
    float *data = get_sensordata();
    LatestReading = ("D*" + String(id) + String(data[0]) + " " + String(data[1]) + " " + String(data[2]));
    Serial.println("testReading :" + LatestReading);
    debug("DEBUG - DATA P(" + String(data[0]) + ") T(" + String(data[1]) + ") H(" + String(data[2])+ ")");
  }

  //Setup time for master node
  if(master_node == true){
    Serial.println("Enter current time");
    while (Serial.available() == 0);
    String str = Serial.readStringUntil('\n');
    setTime(str.toInt());
    
  }
  delay(5000);

 

}

void loop() {

    //Time Keeping
    time_t TimeNow = now();
    int Time = TimeNow;

    //Check commands from PC gui

    

    
    if (gui_receive() == 6) setup();
    

    if (master_node == true) {
      if (Time%ListenFreq < 2){
        if (currentTurnID > 6) currentTurnID = 0;
        else currentTurnID += 1;
      }
      
      MasterReceiver();
    }

    //Startup Sync - Ran on startup to sync time with nearby nodes
    else if (Synced == false && master_node == false) {
      debug("Startup Sync - ID: " + String(id) + " TIME:" + String(second(now())) + " Init: " + String(ms_initiator));
      StartupSync(); //Ran when NODE is started to sync time with nearby nodes
      confirmation = true;
    }

    
    //Listen Period - Listen for incoming messages + Read/Send sensor data when its the nodes turn
    else if (now()%ListenFreq <= ListenPeriod && master_node == false){ 
      if (confirmation == false && ResendRetries >= 0) {
        Serial.println("Re-sending unconfirmed messages " + String(ResendRetries));
        Transmit_Hex(LastTransmitMsg); //Re-send unconfirmed messages
        ReceiveLoop();
        ResendRetries -= 1;
      }
      else{
        ResendRetries = 1;
        confirmation = true;
        if ((id-currentTurnID)%6==0){ //Read sensors and send data
          debug("Local Sync - ID: " + String(id) + " TIME:" + String(second(now())) + " Init: " + String(ms_initiator));
    
          //Read and save sensor data to sd-card *TESTING*
          float *data = get_sensordata();
          debug("DEBUG - DATA P(" + String(data[0]) + ") T(" + String(data[1]) + ") H(" + String(data[2])+ ")");
          logs("DATA P(" + String(data[0]) + ") T(" + String(data[1]) + ") H(" + String(data[2])+ ")");
          delay(100);
          LatestReading = ("D*" + String(id) + String(data[0]) + " " + String(data[1]) + " " + String(data[2]));
          if (Transmit_String("D*" + String(id) + String(data[0]) + " " + String(data[1]) + " " + String(data[2])) == 0) {
            Serial.println("tx Success");
            confirmation = false;
          }
          else {
            debug("tx Error");
          }
         
          ReceiveLoop();
        
        }
        
        else{ //Listen for incoming data
          debug("Listening - ID: " + String(id) + " TIME:" + String(int(now())) + " Init: " + String(ms_initiator) + " TurnID: " + String(currentTurnID)); 
          ReceiveLoop();
        } 
        
      }
      
    }

    //Sleep - low power mode
    else if (Synced == true && master_node == false) {

      //switch to the next set of IDs. Nodes in this group will now send their data
      if (currentTurnID > 6) currentTurnID = 0;
      else currentTurnID += 1;
      
      loraSerial.println(F("sys sleep 8")); //Put LoRa to sleep
      wait_for_ok();

      debug("SLEEP");
      delay(10150); 
    }
    
    else delay(1000);

    gui_send("NODE", (String(Time)+String(currentTurnID))); //Keep-Alive GUI

}

void ReceiveLoop() {
  int out;
  if ((out = Receive_String(Synced)) == 0){
    Serial.println(F("rx Listening")); //Listening
  }
  else if (out  == 2){
    Serial.println(F("rx Data")); //Data
  }
  else if (out  == 3){
    Serial.println(F("rx Confirmation")); //Confirmation
  }
  else if (out  == 4){
    Serial.println(F("rx Time Sync")); //Confirmation
    Synced = true;
  }
  else{
    Serial.println(F("rx Error")); //Error
  }
}

void StartupSync() {
  int out;
  
  Transmit_Hex(F("3C3C"));
  ReceiveLoop();
  
  SyncAttempt += 1;
  if (SyncAttempt > SyncRetries) {
    while(true) Serial.println(F("Unable to sync")); //Error
    
    
  }
}
