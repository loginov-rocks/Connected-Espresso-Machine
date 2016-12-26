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

String lastCommand;

void setup() {
    Serial.begin(SERIAL_BAUDRATE);

    Serial.print("Initial coffeemaker command: ");
    lastCommand = coffeemaker.getCommand();
    Serial.println(lastCommand);
}

void loop() {
    String command = coffeemaker.getCommand();

    // Print message only if coffeemaker command changes
    if (command != lastCommand) {
        lastCommand = command;
        Serial.print("Coffeemaker switched to command: ");
        Serial.println(command);
    }

    coffeemaker.update();
}

