/**
 * Arduino Coffemaker — Coffeemaker class implementation
 * Created by Danila Loginov, December 26, 2016
 * https://github.com/1oginov/Arduino-Coffeemaker
 */

#include "Coffeemaker.h"

void Coffeemaker::setCommand(String command) {
    _command = command;
}

Coffeemaker::Coffeemaker(int pumpPin, int boilerPin, int isBoilingPin, int isSteamPin, int togglePin) :
    _pump(pumpPin),
    _boiler(boilerPin, isBoilingPin, isSteamPin),
    _toggle(togglePin) {
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

void Coffeemaker::update() {
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

    _boiler.update();
}

