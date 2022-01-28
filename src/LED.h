/*
Copyright of Sanjay Bhatikar
*/
#ifndef LED_H
#define LED_H

#include <Arduino.h>
#include "Pins.h"

class LED
{
    public:
        LED(int);
        void set_level(bool=false);
        bool get_level();
    private:
        bool _level;
        bool _pin;
};

#endif