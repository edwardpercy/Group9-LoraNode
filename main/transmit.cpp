#include "transmit.h"
#include "utilities.h"

int Transmit_String(String input,SoftwareSerial &loraSerial)
{
    String str;
    int str_len = input.length() + 1;
    char char_array[str_len];
    input.toCharArray(char_array, str_len);

    char RBuffer[255];
    sprintf(RBuffer, "00");
    for (int i = 0; i < str_len; ++i) {
    sprintf(RBuffer + strlen(RBuffer), "%02X", char_array[i]);
    }

    char TData[255];
    sprintf(TData, "radio tx %s", RBuffer);
    Serial.println(TData);
    led_on();
    loraSerial.println(TData);
    led_off();

    return wait_for_ok_plus_reset(loraSerial);
}
