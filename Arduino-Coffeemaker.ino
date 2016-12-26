/**
 * Arduino Coffemaker
 * Created by Danila Loginov, December 25, 2016
 * https://github.com/1oginov/Arduino-Coffeemaker
 */

#include "Relay.h"
#include "Boiler.h"
#include "Toggle.h"

#define SERIAL_BAUDRATE         9600

#define PUMP_RELAY_PIN          2

#define BOILER_RELAY_PIN        3
#define BOILER_IS_BOILING_PIN   A0
#define BOILER_IS_STEAM_PIN     A1

#define TOGGLE_PIN              A2

Relay pump(PUMP_RELAY_PIN);
Boiler boiler(BOILER_RELAY_PIN, BOILER_IS_BOILING_PIN, BOILER_IS_STEAM_PIN);
Toggle toggle(TOGGLE_PIN);

toggle_state_t lastToggleState;

void setup() {
    Serial.begin(SERIAL_BAUDRATE);

    Serial.print("Initial pump state: ");
    Serial.println(pump.getState() ? "ON" : "OFF");

    Serial.print("Initial boiler state: ");
    Serial.println(boiler.getState() ? "ON" : "OFF");

    Serial.print("Initial toggle state: ");
    lastToggleState = toggle.getState();
    switch (lastToggleState) {
        case BOIL: Serial.println("BOIL"); break;
        case MAKE_STEAM: Serial.println("MAKE_STEAM"); break;
        case POUR_WATER: Serial.println("POUR_WATER"); break;
        default: Serial.println("OFF"); break;
    }
}

void loop() {
    toggle_state_t toggleState = toggle.getState();

    // Print message only if toogle state changes
    if (toggleState != lastToggleState) {
        lastToggleState = toggleState;
        Serial.print("Toggle switched to state: ");
        switch (toggleState) {
            case BOIL: Serial.println("BOIL"); break;
            case MAKE_STEAM: Serial.println("MAKE_STEAM"); break;
            case POUR_WATER: Serial.println("POUR_WATER"); break;
            default: Serial.println("OFF"); break;
        }
    }

    // Do actions depending on toggle state
    switch (toggleState) {
        case BOIL:
            pump.off();
            boiler.setTargetTemp(BOILING);
            break;

        case MAKE_STEAM:
            pump.off();
            boiler.setTargetTemp(STEAM);
            break;

        case POUR_WATER:
            pump.on();
            boiler.setTargetTemp(COLD);
            break;

        default:
            pump.off();
            boiler.setTargetTemp(COLD);
            break;
    }

    boiler.update();
}

