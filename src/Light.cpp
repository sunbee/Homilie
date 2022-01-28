/*
Copyright of Sanjay Bhatikar
*/

#include "Light.h"

Light::Light(int pin_no) {
    this->_pin = pin_no;
    pinMode(this->_pin, OUTPUT);
}

void Light::set_level(bool level) {
    digitalWrite(this->_pin, HIGH);
    this->_level = true;
}

bool Light::get_level() {
    return this->_level;
}