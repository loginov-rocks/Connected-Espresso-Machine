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
    off();
}

void Coffeemaker::off() {
    stopPouringWater();
    coolDown();
    setCommand(__FUNCTION__);
}

void Coffeemaker::pourWater() {
    _pump.on();
    setCommand(__FUNCTION__);
}

void Coffeemaker::stopPouringWater() {
    _pump.off();
    setCommand( __FUNCTION__);
}

void Coffeemaker::coolDown() {
    _boiler.setTargetTemp(COLD);
    setCommand(__FUNCTION__);
}

void Coffeemaker::boil() {
    _boiler.setTargetTemp(BOILING);
    setCommand(__FUNCTION__);
}

void Coffeemaker::makeSteam() {
    _boiler.setTargetTemp(STEAM);
    setCommand(__FUNCTION__);
}

void Coffeemaker::setCommand(String command) {
    _command = command;
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
    switch (_toggle.getState()) {
        case BOIL:
            stopPouringWater();
            boil();
            break;

        case MAKE_STEAM:
            stopPouringWater();
            makeSteam();
            break;

        case POUR_WATER:
            coolDown();
            pourWater();
            break;

        default:
            off();
            break;
    }

    _boiler.update();
}

