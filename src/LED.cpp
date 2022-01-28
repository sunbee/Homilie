/*
Copyright of Sanjay Bhatikar
*/

#include "LED.h"

LED::LED(int pin_no) {
    this->_pin = pin_no;
    pinMode(this->_pin, OUTPUT);
}

void LED::set_level(bool level) {
    digitalWrite(this->_pin, HIGH);
    this->_level = true;
}

bool LED::get_level() {
    return this->_level;
}