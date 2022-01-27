/*
Copyright of Sanjay Bhatikar
*/
#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>
#include "Pins.h"

class Buzzer
{
    public:
        Buzzer(int=PIN_BUZZER);
        void sound_alarm(int=PIN_BUZZER, int=3);
    private:
};

#endif