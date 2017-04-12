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
    _lastReading = 0;
    _toggled = false;
}

toggle_state_t Toggle::getState() {
    if (_lastReading + TOGGLE_DEBOUNCE_TIMEOUT < millis()) {
        _lastReading = millis();

        int reading = analogRead(_pin);
        toggle_state_t state;

        if (reading < TOGGLE_OFF_READING + TOGGLE_READING_DEVIATION) {
            state = OFF;
        }
        else if (reading > TOGGLE_POUR_WATER_READING - TOGGLE_READING_DEVIATION) {
            state = POUR_WATER;
        }
        else if (reading > TOGGLE_BOIL_READING - TOGGLE_READING_DEVIATION &&
                 reading < TOGGLE_BOIL_READING + TOGGLE_READING_DEVIATION) {
            state = BOIL;
        }
        else if (reading > TOGGLE_MAKE_STEAM_READING - TOGGLE_READING_DEVIATION &&
                 reading < TOGGLE_MAKE_STEAM_READING + TOGGLE_READING_DEVIATION) {
            state = MAKE_STEAM;
        }
        else {
            return _lastState;
        }

        if (state != _lastState) {
            _lastState = state;
            _toggled = true;
        }
    }

    return _lastState;
}

boolean Toggle::isToggled() {
    if (!_toggled) {
        return false;
    }

    _toggled = false;
    return true;
}

