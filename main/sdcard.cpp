#include "sdcard.h"

void load_sd()
{

    Sd2Card card;
    
    const int chipSelect = 10;
  
  
    // we'll use the initialization code from the utility libraries
    // since we're just testing if the card is working!
    if (!card.init(SPI_HALF_SPEED, chipSelect)) {
      Serial.println(F("SD: FAIL"));
      //while (1);
    } else {
      
      Serial.println(F("SD: C"));
    }
  

  
}

void logs(String info){

    File Diag;
    
    if (!SD.begin(10)) {

      //while (1);
    }
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    Diag = SD.open("log.txt", FILE_WRITE);
  
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
