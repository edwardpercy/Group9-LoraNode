#include "transmit.h"
#include "utilities.h"

int Transmit_String(String input,SoftwareSerial &loraSerial) //MAX 50 char length
{
    String str;
    int str_len = input.length() + 1;
    char char_array[str_len];
    input.toCharArray(char_array, str_len);


    char RBuffer[(str_len*2)+5];

    sprintf(RBuffer, "00");

    for (int i = 0; i < str_len; ++i) {
    sprintf(RBuffer + strlen(RBuffer), "%02X", char_array[i]);
    }


    char TData[(str_len*2)+15];

    sprintf(TData, "radio tx %s", RBuffer);
    Serial.println(TData);
    loraSerial.println(TData);

    return wait_for_ok_plus_reset(loraSerial);
}

int Transmit_LastSync(SoftwareSerial &loraSerial) //MAX 50 char length
{
    int SyncFreq = 1000;
    time_t TimeNow = now();
    int LastSync = TimeNow%SyncFreq;
    
    String input = String(LastSync);
    input = String(id) + input;
    String str;
    int str_len = input.length() + 1;
    char char_array[str_len];
    input.toCharArray(char_array, str_len);


    char RBuffer[(str_len*2)+5];


    sprintf(RBuffer, "3E3E");

    for (int i = 0; i < str_len; ++i) {
    sprintf(RBuffer + strlen(RBuffer), "%02X", char_array[i]);
    }

    char TData[(str_len*2)+15];

    sprintf(TData, "radio tx %s", RBuffer);
    Serial.println(TData);
    loraSerial.println(TData);
    ms_initiator = false;
    return wait_for_ok_plus_reset(loraSerial);
}

int Transmit_Hex(String input,SoftwareSerial &loraSerial) //MAX 50 Hex 
{
    String str;
    int str_len = input.length() + 1;
    char char_array[str_len];
    input.toCharArray(char_array, str_len);


    char TData[(str_len*2)];
    sprintf(TData, "radio tx %s", char_array);
    loraSerial.println(TData);

    return wait_for_ok_plus_reset(loraSerial);
}
