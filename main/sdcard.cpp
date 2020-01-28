#include "sdcard.h"

void load_sd()
{
  Sd2Card card;
  SdVolume volume;
  SdFile root;
  
  const int chipSelect = 10;


  // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println(F("\nSD: FAIL"));
    while (1);
  } else {
    
    Serial.println(F("\nSD: CONNECTED"));
  }

  
}

void logs(String info){
  File Diag;
  
  if (!SD.begin(10)) {
    while (1);
  }

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  Diag = SD.open("diagnostics.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (Diag) {
    Diag.println(info);
    // close the file:
    Diag.close();
  } else {
    Serial.println("error opening test.txt");
  }
  SD.end();
}

void write_sd(){
  File myFile;
  
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("testing 1, 2, 3.");
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  SD.end();
}
