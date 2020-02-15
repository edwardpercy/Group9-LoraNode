#include "sdcard.h"


void logs(String info){

    File Diag;
    
    if (!SD.begin(10)) {

      //while (1);
    }

    Diag = SD.open(F("log.txt"), FILE_WRITE);
  
    // if the file opened okay, write to it:
    if (Diag) {
      Diag.println(String(now()) + " - " + info);
      // close the file:
      Diag.close();
   
    } else {
      // if the file didn't open, print an error:

    }
    SD.end();
  
}
