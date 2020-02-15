#include "receive.h"

bool TimeAdjusted = false;

int Receive_String(bool Synced){
  String str;
 
  loraSerial.println(F("radio rx 0")); //wait for 2 seconds to receive
  str = loraSerial.readStringUntil('\n');
  
  if ( str.indexOf(F("ok")) == 0 )
  {
    str = String("");
    while (str == "")
    {
      str = loraSerial.readStringUntil('\n');
    }
    if ( str.indexOf(F("radio_rx")) == 0 )
    {
      str.remove(0, 10);
      if (TimeAdjusted == true){
        TimeAdjusted = false;
        return 4;
      }
      else if (str.indexOf(F("004000")) == 0) return 3;
      else{
        if (str.indexOf(F("3C3C")) != 0 && Synced == true) Transmit_Hex("004000");
        ProcessMessage(Synced,str);
        if (TimeAdjusted == true){
          TimeAdjusted = false;
          return 4;
        }
        return 2;
      }
    }
    else
    {
      return 0;
    }
  }
  else
  {
    Serial.println(F("rx E"));
    return 1;
  }
}

char h2c(char c1, char c2)
{
  char output = 0;
  if (c1 <= '9' && c1 >= '0') output += c1 - '0';
  if (c1 <= 'f' && c1 >= 'a') output += c1 - 'a' + 0xA;
  if (c1 <= 'F' && c1 >= 'A') output += c1 - 'A' + 0xA;
  output *= 0x10;
  if (c2 <= '9' && c2 >= '0') output += c2 - '0';
  if (c2 <= 'f' && c2 >= 'a') output += c2 - 'a' + 0xA;
  if (c2 <= 'F' && c2 >= 'A') output += c2 - 'A' + 0xA;

  return output;
}

String ProcessMessage(bool Synced,String str) {
  
  Serial.println(str);
  int str_len = str.length() + 1;
  char char_array[str_len];
  str.toCharArray(char_array, str_len);


  char ReceivedChars[(str_len*2)+10];



  for (int i = 0; i < str_len; i += 2)  {
    if (i == 0) {
      sprintf(ReceivedChars, "%c", (h2c(char_array[i], char_array[i + 1])));
    }
    else {
      sprintf(ReceivedChars + strlen(ReceivedChars), "%c", (h2c(char_array[i], char_array[i + 1])));
    }
  }

  
  if ( str.indexOf(F("3C3C")) == 0 && Synced == true) Transmit_LastSync(); //Send Sync Data
  if ( str.indexOf(F("3E3E")) == 0 ) SyncTime(ReceivedChars); //Sync with received sync data
  if ( str.indexOf(F("2A43")) == 0 ){ //Received temperature data

    String RChars = String(ReceivedChars);
    String rid = String(RChars[0]);
    RChars.remove(0, 1);

    Serial.println(String(RChars) + "°C @ NODE" + rid);
  }


  return ReceivedChars;
}

void SyncTime(String ReceivedLastSync){
  ReceivedLastSync.remove(0, 2);
  String receiveID = String(ReceivedLastSync[0]);
 
  int rid = receiveID.toInt();
  rid += 1;

  if (rid > 9) id = 0;
  else id = rid;

  ReceivedLastSync.remove(0, 1);
  int SyncFreq = 1000;
  time_t TimeNow = now();
  int LastSync = TimeNow%SyncFreq;
  
  adjustTime((ReceivedLastSync.toInt()+1)-LastSync); 
  TimeAdjusted = true;
  Serial.println(F("rx TS"));
}
