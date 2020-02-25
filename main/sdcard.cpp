#include "sdcard.h"



int SDCheck(){

  SdVolume volume;
  Sd2Card card;
  if (!card.init(SPI_HALF_SPEED, PA8)) {
    Serial.println("SD: Connection Error");
    return 1;
  } 
  else if (!volume.init(card)) {
    Serial.println("SD: Format Error");
    return 2;
  }
  else {
    Serial.println("SD: Success");
    return 0;
  }  
  
}

void logs(String info){

    File Diag;
  
    if (!SD.begin(PA8)) {

      //while (1);
    }

    Diag = SD.open(F("log.txt"), FILE_WRITE);
  
    // if the file opened okay, write to it:
    if (Diag) {
    
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
