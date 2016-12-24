/**
 * Arduino Coffemaker — Relay class implementation
 * Created by Danila Loginov, December 25, 2016
 * https://github.com/1oginov/Arduino-Coffeemaker
 */

#include "Relay.h"

Relay::Relay(int pin) {
    _pin = pin;
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
}

boolean Relay::getState() {
    return digitalRead(_pin);
}

void Relay::on() {
    digitalWrite(_pin, HIGH);
}

void Relay::off() {
    digitalWrite(_pin, LOW);
}

void Relay::toggle() {
    digitalWrite(_pin, !getState());
}

