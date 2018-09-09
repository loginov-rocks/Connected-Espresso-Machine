/**
 * Arduino Coffemaker — Boiler class implementation
 * Created by Danila Loginov, December 25, 2016
 * https://github.com/1oginov/Arduino-Coffeemaker
 */

#include "Boiler.h"

Boiler::Boiler(int relayPin, int isBoilingPin, int isSteamPin) : Relay (relayPin) {
    _isBoilingPin = isBoilingPin;
    _isSteamPin = isSteamPin;

    pinMode(_isBoilingPin, INPUT_PULLUP);
    pinMode(_isSteamPin, INPUT_PULLUP);

    setTargetTemp(COLD);
}

boolean Boiler::getState() {
    return Relay::getState();
}

temp_t Boiler::getTemp() {
    if (digitalRead(_isSteamPin)) {
        return STEAM;
    }

    if (digitalRead(_isBoilingPin)) {
        return BOILING;
    }

    return COLD;
}

temp_t Boiler::getTargetTemp() {
    return _targetTemp;
}

void Boiler::setTargetTemp(temp_t targetTemp) {
    _targetTemp = targetTemp;
}

void Boiler::update() {
    temp_t temp = getTemp(),
           targetTemp = getTargetTemp();

    if (targetTemp == COLD || temp == targetTemp || (temp == STEAM && targetTemp == BOILING)) {
        off();
    }
    else {
        on();
    }
}

