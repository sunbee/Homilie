/*
Copyright of Sanjay R. Bhatikar
*/
#include "Deserializer.h"

Deserializer::Deserializer() {

}

Payload Deserializer::deserialize(char *PAYLOAD, unsigned int length) {
    /*
    Deserialize the serialized JSON read from topic on MQTT broker.
    The array of char is read one character at a time to extract the 
    key-value pairs and populate the struct.
    Uses the fact that JSON payload has enclosing curly-braces, with
    comma-separated key-value pairs, a colon separating key and value,
    and text enclosed in quotes. 
    EXPECTS ONLY INTEGER VALUES!!! KEYS MUST BE CONSISTENT WITH PAYLOAD STRUCT!!!
    */
    String KEY;
    String VAL;
    int ch;

    for (int i = 0; i < length; i++) {
        ch = PAYLOAD[i];
        if ((ch == '{') || (ch == 32) || (ch == 34) || (ch == 39)) { // Discard: curly brace, whitespace, quotation marks
            // Do nothing!
        } else if (ch == ':') {
            // Print the key now if you need to. Otherwise do nothing.
        } else if ((ch == 44) || (ch == '}')) { // comma
            if (KEY == "Lo") {
                this->_data.Lo = VAL.toInt();
            } else if (KEY == "Me") {
                this->_data.Me = VAL.toInt();
            } else if (KEY == "Hi") {
                this->_data.Hi = VAL.toInt();
            }
            KEY = "";
            VAL = "";
        } else if ((ch >= '0') && (ch <= '9')) {
            VAL += char(ch);
        } else if (((ch >= 65) && (ch <= 90)) || ((ch > 97) && (ch < 122))) { // A-Z or a-z
            KEY += char(ch);
        }
    } 
    return this->_data;
}
