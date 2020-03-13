#include "serial.h"

void debug(String message){
  if (show_debug == true) Serial.println("DEBUG - " + message);
}

int gui_receive(){ //Receive commands from the PC gui
  if (Serial.available() > 0) {
      // read the incoming byte:
      String str = Serial.readStringUntil('\n');
      
      if (str == "RDATA"){ //Send SD-Card data to GUI
        gui_send("DATA", String(readSD()));
        return 2;     
      }
      
      return 1;
  }
  return 0;
}

void gui_send(String type, String message){ //Send commands to the PC gui
  if (master_node == true) Serial.println(type + message);
}
