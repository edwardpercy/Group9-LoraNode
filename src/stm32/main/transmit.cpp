#include "transmit.h"
#include "utilities.h"

int Transmit_String(String input) //MAX 50 char length
{
    String str;
    int str_len = input.length() + 1;
    char char_array[str_len];
    input.toCharArray(char_array, str_len);


    char RBuffer[(str_len*2)+5];

    for (int i = 0; i < str_len; ++i) {
    sprintf(RBuffer + strlen(RBuffer), "%02X", char_array[i]);
    }


    return Transmit_Hex(RBuffer);
}

int Transmit_LastSync() //MAX 50 char length
{
 
    time_t TimeNow = now();
    int LastSync = TimeNow%MasterSyncFreq;
    
    String input = String(LastSync);
    input = String(id) + String(currentTurnID) + input;
    String str;
    int str_len = input.length() + 1;
    char char_array[str_len];
    input.toCharArray(char_array, str_len);


    char RBuffer[(str_len*2)+5];


    sprintf(RBuffer, "3E3E");

    for (int i = 0; i < str_len; ++i) {
    sprintf(RBuffer + strlen(RBuffer), "%02X", char_array[i]);
    }

    return Transmit_Hex(RBuffer);
}

int Transmit_Hex(String input) //MAX 50 Hex 
{
    String str;
    int str_len = input.length() + 1;
    char char_array[str_len];
    input.toCharArray(char_array, str_len);


    char TData[(str_len*2)];
    sprintf(TData, "radio tx %s", char_array);
    loraSerial.println(TData);

    LastTransmitMsg = TData;
    confirmation = false;

    return wait_for_ok_plus_reset();
}