/**
 * Arduino Coffemaker — Coffeemaker class implementation
 * Created by Danila Loginov, December 26, 2016
 * https://github.com/1oginov/Arduino-Coffeemaker
 */

#include "Coffeemaker.h"

void Coffeemaker::setCommand(String command) {
    _command = command;

    // Reset metrics related to making coffee command
    if (_command != "makeCoffee") {
        _millisLeftToMakeCoffee = 0;
        _pourWaterStartMillis = 0;
    }
}

Coffeemaker::Coffeemaker(int pumpPin, int boilerPin, int isBoilingPin, int isSteamPin, int togglePin, int donePin) :
    _pump(pumpPin),
    _boiler(boilerPin, isBoilingPin, isSteamPin),
    _toggle(togglePin) {

    _donePin = donePin;
    pinMode(_donePin, OUTPUT);
    digitalWrite(_donePin, LOW);

    setCommand("off");
}

boolean Coffeemaker::off() {
    if (getToggleState() != OFF) {
        return false;
    }

    _pump.off();
    _boiler.setTargetTemp(COLD);

    setCommand(__FUNCTION__);
    return true;
}

boolean Coffeemaker::pourWater() {
    if (getToggleState() != OFF) {
        return false;
    }

    _pump.on();

    setCommand(__FUNCTION__);
    return true;
}

boolean Coffeemaker::stopPouringWater() {
    if (getToggleState() != OFF) {
        return false;
    }

    _pump.off();

    setCommand(__FUNCTION__);
    return true;
}

boolean Coffeemaker::coolDown() {
    if (getToggleState() != OFF) {
        return false;
    }

    _boiler.setTargetTemp(COLD);

    setCommand(__FUNCTION__);
    return true;
}

boolean Coffeemaker::boil() {
    if (getToggleState() != OFF) {
        return false;
    }

    _boiler.setTargetTemp(BOILING);

    setCommand(__FUNCTION__);
    return true;
}

boolean Coffeemaker::makeSteam() {
    if (getToggleState() != OFF) {
        return false;
    }

    _boiler.setTargetTemp(STEAM);

    setCommand(__FUNCTION__);
    return true;
}

boolean Coffeemaker::makeCoffee(int seconds) {
    if (seconds <= 0 || getToggleState() != OFF) {
        return false;
    }

    _pump.off();
    _boiler.setTargetTemp(BOILING);

    _millisLeftToMakeCoffee = (long) seconds * 1000;
    _pourWaterStartMillis = 0;

    setCommand(__FUNCTION__);
    return true;
}

String Coffeemaker::getCommand() {
    return _command;
}

boolean Coffeemaker::getPumpState() {
    return _pump.getState();
}

boolean Coffeemaker::getBoilerState() {
    return _boiler.getState();
}

temp_t Coffeemaker::getTemp() {
    return _boiler.getTemp();
}

temp_t Coffeemaker::getTargetTemp() {
    return _boiler.getTargetTemp();
}

toggle_state_t Coffeemaker::getToggleState() {
    return _toggle.getState();
}

long Coffeemaker::getMillisLeftToMakeCoffee() {
    if (_pourWaterStartMillis > 0) {
        return _millisLeftToMakeCoffee - millis() + _pourWaterStartMillis;
    }

    return _millisLeftToMakeCoffee;
}

boolean Coffeemaker::isDone() {
    if (getCommand() == "makeCoffee" && _isDone) {
        off();
        _isDone = false;
        return true;
    }

    return _isDone;
}

void Coffeemaker::update() {

    // Get toggle state and execute command if it has been toggled
    toggle_state_t toggleState = getToggleState();

    if (_toggle.isToggled()) {
        switch (toggleState) {
            case BOIL:
                _pump.off();
                _boiler.setTargetTemp(BOILING);
                setCommand("boil");
                break;

            case MAKE_STEAM:
                _pump.off();
                _boiler.setTargetTemp(STEAM);
                setCommand("makeSteam");
                break;

            case POUR_WATER:
                _pump.on();
                _boiler.setTargetTemp(COLD);
                setCommand("pourWater");
                break;

            default:
                off();
                break;
        }
    }

    // Update boiler state, toggle heating depending on demands
    _boiler.update();

    // Making coffee
    if (_millisLeftToMakeCoffee > 0) {

        // Water pouring has been already started
        if (_pourWaterStartMillis > 0) {

            // Time to make coffee is over
            if (_pourWaterStartMillis + _millisLeftToMakeCoffee < millis()) {
                _pump.off();
                _boiler.setTargetTemp(COLD);
                _millisLeftToMakeCoffee = 0;
                _pourWaterStartMillis = 0;
            }

            // Temperature is not suitable, suspend making coffee and update left time
            else if (getTemp() != BOILING) {
                _pump.off();
                _millisLeftToMakeCoffee -= millis() - _pourWaterStartMillis;
                _pourWaterStartMillis = 0;
            }

        }

        // Suitable temperature, start to pour water
        else if (getTemp() == BOILING) {
            _pump.on();
            _pourWaterStartMillis = millis();
        }

    }

    // Indicate commands execution by done LED
    _isDone = false;
    if (getCommand() == "makeCoffee") {
        if (_millisLeftToMakeCoffee == 0) {
            _isDone = true;
        }
    }
    else if (getTargetTemp() != COLD && getTemp() == getTargetTemp()) {
        _isDone = true;
    }
    digitalWrite(_donePin, _isDone);

}

