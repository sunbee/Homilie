/*
Copyright of Sanjay Bhatikar
*/

#include "Light.h"

Light::Light(int pin_no) {
    this->_pin = pin_no;
    pinMode(this->_pin, OUTPUT);
}

void Light::set_level(bool level) {
    if (level) {
        digitalWrite(this->_pin, HIGH);
        this->_level = true;
    } else {
        digitalWrite(this->_pin, LOW);
        this->_level = false;
    }
}

bool Light::get_level() {
    return this->_level;
}