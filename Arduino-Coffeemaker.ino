/**
 * Arduino Coffemaker
 * Created by Danila Loginov, December 25, 2016
 * https://github.com/1oginov/Arduino-Coffeemaker
 */

#include "Coffeemaker.h"

#define SERIAL_BAUDRATE         9600

#define PUMP_RELAY_PIN          2

#define BOILER_RELAY_PIN        3
#define BOILER_IS_BOILING_PIN   A0
#define BOILER_IS_STEAM_PIN     A1

#define TOGGLE_PIN              A2

Coffeemaker coffeemaker(PUMP_RELAY_PIN, BOILER_RELAY_PIN, BOILER_IS_BOILING_PIN, BOILER_IS_STEAM_PIN, TOGGLE_PIN);

toggle_state_t lastToggleState;

void setup() {
    Serial.begin(SERIAL_BAUDRATE);

    Serial.print("Initial pump state: ");
    Serial.println(coffeemaker.getPumpState() ? "ON" : "OFF");

    Serial.print("Initial boiler state: ");
    Serial.println(coffeemaker.getBoilerState() ? "ON" : "OFF");

    Serial.print("Initial toggle state: ");
    lastToggleState = coffeemaker.getToggleState();
    switch (lastToggleState) {
        case BOIL: Serial.println("BOIL"); break;
        case MAKE_STEAM: Serial.println("MAKE_STEAM"); break;
        case POUR_WATER: Serial.println("POUR_WATER"); break;
        default: Serial.println("OFF"); break;
    }
}

void loop() {
    toggle_state_t toggleState = coffeemaker.getToggleState();

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

    coffeemaker.update();
}

