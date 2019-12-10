#include <Time.h>
#include <TimeLib.h>
#include <SoftwareSerial.h>
#include <stdio.h>
#include <string.h>
#include "transmit.h"
#include "utilities.h"

SoftwareSerial loraSerial(10, 11);

void setup() {
  int Startup_Check = 0;
  String str;
  //output LED pin
  pinMode(13, OUTPUT);
  led_off();

  Serial.begin(57600);

  loraSerial.begin(9600);
  loraSerial.setTimeout(1000);
  lora_autobaud(loraSerial);

  led_on();
  delay(1000);
  led_off();

  loraSerial.listen();
  str = loraSerial.readStringUntil('\n');
  loraSerial.println("sys get ver");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);
  loraSerial.println("mac pause");
  str = loraSerial.readStringUntil('\n');
  Serial.println(str);

  Serial.println("NODE: Starting");
  loraSerial.println("radio set mod lora");
  Startup_Check += wait_for_ok(loraSerial);
  loraSerial.println("radio set freq 869100000");
  Startup_Check += wait_for_ok(loraSerial);
  loraSerial.println("radio set pwr 14");
  Startup_Check += wait_for_ok(loraSerial);
  loraSerial.println("radio set sf sf7");
  Startup_Check += wait_for_ok(loraSerial);
  loraSerial.println("radio set afcbw 41.7");
  Startup_Check += wait_for_ok(loraSerial);
  loraSerial.println("radio set rxbw 125");
  Startup_Check += wait_for_ok(loraSerial);
  loraSerial.println("radio set prlen 8");
  Startup_Check += wait_for_ok(loraSerial);
  loraSerial.println("radio set crc on");
  Startup_Check += wait_for_ok(loraSerial);
  loraSerial.println("radio set iqi off");
  Startup_Check += wait_for_ok(loraSerial);
  loraSerial.println("radio set cr 4/5");
  Startup_Check += wait_for_ok(loraSerial);
  loraSerial.println("radio set wdt 2000"); //disable for continuous reception (Currently: 2s)
  Startup_Check += wait_for_ok(loraSerial);
  loraSerial.println("radio set sync 12");
  Startup_Check += wait_for_ok(loraSerial);
  loraSerial.println("radio set bw 125");
  Startup_Check += wait_for_ok(loraSerial);

  if (Startup_Check > 0) Serial.println("NODE: Startup Failure");
  else Serial.println("NODE: Startup Success");
}

void loop() {

    if (Serial.available() > 0) { //Read from serial monitor and send over UART LoRa wireless module
        String input = Serial.readStringUntil('\n');
        delay(1000);
        if (Transmit_String(input,loraSerial) == 0) Serial.println("radio tx OK");
        else Serial.println("radio tx ERROR");
      }
}
