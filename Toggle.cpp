/**
 * Arduino Coffemaker — Toggle class implementation
 * Created by Danila Loginov, December 26, 2016
 * https://github.com/1oginov/Arduino-Coffeemaker
 */

#include "Toggle.h"

Toggle::Toggle(int pin) {
    _pin = pin;
    pinMode(_pin, INPUT);

    _lastState = OFF;
}

toggle_state_t Toggle::getState() {
    int reading = analogRead(_pin);

    if (reading < TOGGLE_OFF_READING + TOGGLE_READING_DEVIATION) {
        _lastState = OFF;
    }
    else if (reading > TOGGLE_POUR_WATER_READING - TOGGLE_READING_DEVIATION) {
        _lastState = POUR_WATER;
    }
    else if (reading > TOGGLE_BOIL_READING - TOGGLE_READING_DEVIATION &&
             reading < TOGGLE_BOIL_READING + TOGGLE_READING_DEVIATION) {
        _lastState = BOIL;
    }
    else if (reading > TOGGLE_MAKE_STEAM_READING - TOGGLE_MAKE_STEAM_READING &&
             reading < TOGGLE_MAKE_STEAM_READING + TOGGLE_MAKE_STEAM_READING) {
        _lastState = MAKE_STEAM;
    }

    return _lastState;
}

