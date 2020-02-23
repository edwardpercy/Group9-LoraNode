#include "sdcard.h"


void logs(String info){

    File Diag;
    Serial.println("SD start");
    if (!SD.begin(PA8)) {

      //while (1);
    }

    Diag = SD.open(F("log.txt"), FILE_WRITE);
  
    // if the file opened okay, write to it:
    if (Diag) {
      Serial.println("SD write");
      int t = now();
      Diag.println(String(t) + " - " + info);
      // close the file:
      Diag.close();
   
    } else {
      // if the file didn't open, print an error:
      Serial.println("SD fail");
    }
    SD.end();
  
}
