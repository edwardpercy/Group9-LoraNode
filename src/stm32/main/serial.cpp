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
      
      if (str == "MSYNC"){ //Initiate Master Sync
        Transmit_Hex("4D53");
        int Timeout = 0;
        while (Wait_For_Confirm() != 1 && Timeout < 5){
          gui_send("SYNC", "Sending, Attempt: " + String(Timeout));
          Wait_For_Confirm("4D53");
          Timeout += 1;
        }
        if (Timeout >= 5) gui_send("SYNC", "Timeout Reached");
        else gui_send("SYNC", "Sync Sent");
        
        return 3;     
      }

      if (str.indexOf("STIME") == 0){ //Set time -> Master Node
        str.remove(0, 5);
        setTime(str.toInt());
        return 4;     
      }
      if (str == "RTIME"){ //Request time -> GUI
        gui_send("TIME", String(now()));
        return 5;     
      }
      
      return 1;
  }
  return 0;
}

void gui_send(String type, String message){ //Send commands to the PC gui
  if (master_node == true) Serial.println(type + message);
}
