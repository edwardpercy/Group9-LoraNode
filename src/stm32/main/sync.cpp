#include "sync.h"

std::queue<String> RelayReadings;

void master_sync(){
  
  if (ms_initiator != true) relay_master_signal(30);
  else relay_master_signal(3);
  while(sync_active == true){
    if (ms_initiator == true || !RelayReadings.empty()){
      
      RelayReadings.push(LatestReading);
      
      
      
      while(!RelayReadings.empty()) {
        Serial.println("Popping " + RelayReadings.front());
      
        Transmit_String(RelayReadings.front());
        
        
        
        int Timeout = 0;
        while (Wait_For_Confirm_id() != 1 && Timeout < 100){
          delay(random(0,2000));
          debug(("MASTER SYNC Sending, Attempt: " + String(Timeout)));
          Transmit_String(RelayReadings.front());
          
          Timeout += 1;
        }
        if (Timeout >= 100) debug("MASTER SYNC Timeout Reached");
        else debug("MASTER SYNC Data Sent");
        RelayReadings.pop();
        ms_initiator = false;
      }
   
      
    }
    
    slaveReceiver();  
    
  }
  
}

void relay_master_signal(int timeout_val){
  Transmit_Hex("4D53");
  int Timeout = 0;
  while (Wait_For_Confirm() != 1 && Timeout < timeout_val){
    debug(("RELAY Sending, Attempt: " + String(Timeout)));
    Transmit_Hex("4D53");
    
    Timeout += 1;
  }
  if (Timeout >= timeout_val) {
    debug("RELAY Timeout Reached");
    ms_initiator = true;
  }
  else {
    debug("RELAY Sync Sent");  
   
  }
}

int slaveReceiver(){
  String str;
  loraSerial.println("radio rx 0"); //wait for 60 seconds to receive
  str = loraSerial.readStringUntil('\n');
 
  if ( str.indexOf(F("ok")) == 0 )
  {
   
    str = String("");
    while (str == "")
    {
      str = loraSerial.readStringUntil('\n');
    }


    if ( str.indexOf(F("radio_rx")) == 0 )
    {
      
      str.remove(0, 10);
      
      if ( str.indexOf(F("442A")) == 0 ){
        std::vector<String> Readings;
        Readings = receive_readings(str);
        if (Readings[0].toInt() >= id){
          RelayReadings.push(Readings[1]);
        }
      }

      if ( str.indexOf(F("4553")) == 0 ){
        sync_active = false;
        Transmit_Hex(F("004000"));
        
      }
      
      
//      if ( str.indexOf(F("3C3C")) == 0) {
//        Transmit_LastSync(); //Send Sync Data        
//      }
       
    }
    else{
      debug("MASTER SYNC: Listening");
      return 0;
      }
  }
  else
  {
    Serial.println(str);
    debug("radio not going into receive mode");
    delay(4000);
    return 2;
  }
  
}
