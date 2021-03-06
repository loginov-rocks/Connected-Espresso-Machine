#include <Arduino.h>
#include "Coffeemaker.h"

#define SERIAL_BAUDRATE 9600

#define PUMP_RELAY_PIN D1

#define BOILER_RELAY_PIN D2
#define BOILER_IS_BOILING_PIN D5
#define BOILER_IS_STEAM_PIN D6

#define TOGGLE_PIN A0

#define DONE_PIN D7

Coffeemaker coffeemaker(PUMP_RELAY_PIN, BOILER_RELAY_PIN, BOILER_IS_BOILING_PIN, BOILER_IS_STEAM_PIN, TOGGLE_PIN, DONE_PIN);

String lastCommand;

void setup()
{
    Serial.begin(SERIAL_BAUDRATE);

    Serial.print("Initial coffeemaker command: ");
    lastCommand = coffeemaker.getCommand();
    Serial.println(lastCommand);
}

void loop()
{
    // Get the user's input and try to execute the requested command.
    if (Serial.available())
    {
        int userInput = Serial.read();

        boolean isCommandCorrect = true;
        String userCommand;
        boolean result;

        switch (userInput)
        {
        case '0':
            result = coffeemaker.off();
            userCommand = "off";
            break;
        case '1':
            result = coffeemaker.pourWater();
            userCommand = "pour water";
            break;
        case '2':
            result = coffeemaker.stopPouringWater();
            userCommand = "stop pouring water";
            break;
        case '3':
            result = coffeemaker.coolDown();
            userCommand = "cool down";
            break;
        case '4':
            result = coffeemaker.boil();
            userCommand = "boil";
            break;
        case '5':
            result = coffeemaker.makeSteam();
            userCommand = "make steam";
            break;
        case '6':
            result = coffeemaker.makeCoffee(60);
            userCommand = "make coffee for 60 seconds";
            break;
        default:
            isCommandCorrect = false;
            break;
        }

        if (isCommandCorrect)
        {
            Serial.print("Try to ");
            Serial.print(userCommand);
            Serial.print(": ");
            Serial.println(result ? "success" : "fail");
        }
    }

    String command = coffeemaker.getCommand();

    // Print message only if coffeemaker command changes.
    if (command != lastCommand)
    {
        lastCommand = command;
        Serial.print("Coffeemaker switched to command: ");
        Serial.println(command);
    }

    if (command == "makeCoffee")
    {
        Serial.print("Milliseconds left to make coffee: ");
        Serial.println(coffeemaker.getMillisLeftToMakeCoffee());
    }

    coffeemaker.work();
}
