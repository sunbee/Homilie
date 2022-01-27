/*
Copyright of Sanjay Bhatikar
*/
#include "Buzzer.h"

Buzzer::Buzzer(int pin) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
    delay(250);
}

void Buzzer::sound_alarm(int pin, int cycles) {
    digitalWrite(pin, LOW);
    for (int i=0; i < cycles; i++) {
        tone(pin, 523) ; //DO note 523 Hz
        delay (250); 
        tone(pin, 587) ; //RE note ...
        delay (250); 
        tone(pin, 659) ; //MI note ...
        delay (250);
        noTone(pin);
        delay(250);
    }
    digitalWrite(pin, HIGH);
}