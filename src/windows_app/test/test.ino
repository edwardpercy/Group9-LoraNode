void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }
  Serial.println("nodeStart");
}

void loop() {
  if (Serial.available() > 0) {
    // read the incoming byte:
    String str = Serial.readStringUntil('\n');
    Serial.println("Sending Requested Data");
    if (str == "datar"){
      for (int x = 0;x < 1000;x ++){
        Serial.println("Value of x is: " + String((x*x)+x));
      }
      
    }
  }
  
  Serial.println("syncLocal time is 10004345 UNIX");
  delay(1000);
  Serial.println("data4 degrees and 4000 pressure");
  delay(2000);
  Serial.println("nodeCycle Restarted");
}
