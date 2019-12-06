void Transmit_String(String input)
{
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
    str = loraSerial.readStringUntil('\n');
    str = loraSerial.readStringUntil('\n');
    led_off();
}
