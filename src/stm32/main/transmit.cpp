#include "transmit.h"
#include "utilities.h"

int Transmit_String(String input_c)
{
  String input(input_c); // Make a deep copy to be able to do trim()
  input.trim();
  const size_t inputLength = input.length();
  String output;
  output.reserve(inputLength * 2);
  
  for(size_t i = 0; i < inputLength; ++i)
  {
    if(input[i] == '\0') break;

    char buffer[3];
    sprintf(buffer, "%02x", static_cast<int>(input[i]));
    output += buffer[0];
    output += buffer[1];
  }
  return Transmit_Hex(output);
}

int Transmit_LastSync() //MAX 50 char length
{
 
    time_t TimeNow = now();
    int SyncTime = now();
    String input = (String(id) + String(currentTurnID) + String(SyncTime));
    Serial.println(input);
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
    
    LastTransmitMsg = input;
    String str;
    int str_len = input.length() + 1;
    char char_array[str_len];
    input.toCharArray(char_array, str_len);


    char TData[(str_len*2)];
    sprintf(TData, "radio tx %s", char_array);
    loraSerial.println(TData);
    Serial.println("tx sent: " + String(TData));
    return wait_for_ok_plus_reset();
}
