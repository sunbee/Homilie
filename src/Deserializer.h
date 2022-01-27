/*
Copyright of Sanjay Bhatikar
*/
#ifndef DESERIALIZER_H
#define DESERIALIZER_H

#include <Arduino.h>

struct Payload {
    bool Lo;
    bool Me;
    bool Hi; 
};

class Deserializer
{
    public:
        Deserializer();
        Payload deserialize(char* message, unsigned int length);
    private:
        Payload _data;
};

#endif