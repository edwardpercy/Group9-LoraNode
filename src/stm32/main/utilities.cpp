#include "utilities.h"

void lora_autobaud()
{
 
  String response = "";
  while (response == "")
  {
    delay(1000);
    loraSerial.write((byte)0x00);
    loraSerial.write(0x55);
    loraSerial.println();
    loraSerial.println(F("sys get ver"));
    response = loraSerial.readStringUntil('\n');
  }
}
int wait_for_ok_plus_reset()
{

  String str;
  str = loraSerial.readStringUntil('\n');
  Serial.println("tx command: " + str);
  if ( str.indexOf(F("ok")) == 0 ) {
    
    str = String("");
    while (str == "")
    {
      str = loraSerial.readStringUntil('\n');
    }
    Serial.println("tx response: " + str);
    if ( str.indexOf(F("radio_tx_ok")) == 0 ) {
      
      return 0;
    }
    else if ( str.indexOf(F("radio_err")) == 0 ) {
      return 1;
    }
    else{
      return 2;
    }
  }
  
  else{
    return 3;
  }
}

int wait_for_ok()
{

  String str;
  str = loraSerial.readStringUntil('\n');
   
  if ( str.indexOf(F("ok")) == 0 ) {

    return 0;
  }
  
  else{

    return 1;
  }
}
