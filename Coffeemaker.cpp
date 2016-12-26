/**
 * Arduino Coffemaker — Coffeemaker class implementation
 * Created by Danila Loginov, December 26, 2016
 * https://github.com/1oginov/Arduino-Coffeemaker
 */

#include "Coffeemaker.h"

Coffeemaker::Coffeemaker(int pumpPin, int boilerPin, int isBoilingPin, int isSteamPin, int togglePin) :
    _pump(pumpPin),
    _boiler(boilerPin, isBoilingPin, isSteamPin),
    _toggle(togglePin) {
    // Nothing to do here, everything will be done in constructors
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
    switch (_toggle.getState()) {
        case BOIL:
            _pump.off();
            _boiler.setTargetTemp(BOILING);
            break;

        case MAKE_STEAM:
            _pump.off();
            _boiler.setTargetTemp(STEAM);
            break;

        case POUR_WATER:
            _pump.on();
            _boiler.setTargetTemp(COLD);
            break;

        default:
            _pump.off();
            _boiler.setTargetTemp(COLD);
            break;
    }

    _boiler.update();
}

