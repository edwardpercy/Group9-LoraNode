#include "utilities.h"

void lora_autobaud(SoftwareSerial &loraSerial)
{
 
  String response = "";
  while (response == "")
  {
    delay(1000);
    loraSerial.write((byte)0x00);
    loraSerial.write(0x55);
    loraSerial.println();
    loraSerial.println("sys get ver");
    response = loraSerial.readStringUntil('\n');
  }
}
int wait_for_ok_plus_reset(SoftwareSerial &loraSerial)
{

  String str;
  str = loraSerial.readStringUntil('\n');
  if ( str.indexOf("ok") == 0 ) {
    str = loraSerial.readStringUntil('\n');
    return 0;
  }
  
  else{
    str = loraSerial.readStringUntil('\n');
    return 1;
  }
}

int wait_for_ok(SoftwareSerial &loraSerial)
{

  String str;
  str = loraSerial.readStringUntil('\n');
   
  if ( str.indexOf("ok") == 0 ) {
    return 0;
  }
  
  else{
    return 1;
  }
}

void toggle_led()
{
  digitalWrite(13, !digitalRead(13));
}

void led_on()
{
  digitalWrite(13, 1);
}

void led_off()
{
  digitalWrite(13, 0);
}
