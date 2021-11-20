#include "EspressoMachine.h"

#define SERIAL_BAUDRATE 9600

#define PUMP_RELAY_PIN D1
#define BOILER_RELAY_PIN D2
#define BOILER_IS_BOILING_PIN D5
#define BOILER_IS_STEAM_PIN D6
#define TOGGLE_PIN A0
#define DONE_PIN D7

EspressoMachine espressoMachine(PUMP_RELAY_PIN, BOILER_RELAY_PIN, BOILER_IS_BOILING_PIN, BOILER_IS_STEAM_PIN, TOGGLE_PIN, DONE_PIN);

void printlnCommand(EspressoMachineCommand command)
{
    switch (command)
    {
    case EspressoMachineCommand::PourWater:
        Serial.println("POUR_WATER");
        break;

    case EspressoMachineCommand::StopPouringWater:
        Serial.println("STOP_POURING_WATER");
        break;

    case EspressoMachineCommand::Boil:
        Serial.println("BOIL");
        break;

    case EspressoMachineCommand::MakeSteam:
        Serial.println("MAKE_STEAM");
        break;

    case EspressoMachineCommand::CoolDown:
        Serial.println("COOL_DOWN");
        break;

    case EspressoMachineCommand::ToggleBoil:
        Serial.println("TOGGLE_BOIL");
        break;

    case EspressoMachineCommand::ToggleMakeSteam:
        Serial.println("TOGGLE_MAKE_STEAM");
        break;

    case EspressoMachineCommand::TogglePourWater:
        Serial.println("TOGGLE_POUR_WATER");
        break;

    default:
        Serial.println("OFF");
        break;
    }
}

void setup()
{
    Serial.begin(SERIAL_BAUDRATE);

    Serial.print("Initial espresso machine command: ");
    printlnCommand(espressoMachine.getCommand());
}

void loop()
{
    // Set command based on the user input, if available.
    if (Serial.available())
    {
        int userInput = Serial.read();

        boolean result;
        String userCommand;
        boolean isCommandCorrect = true;

        switch (userInput)
        {
        case '0':
            result = espressoMachine.command(EspressoMachineCommand::Off);
            userCommand = "off";
            break;

        case '1':
            result = espressoMachine.command(EspressoMachineCommand::PourWater);
            userCommand = "pour water";
            break;

        case '2':
            result = espressoMachine.command(EspressoMachineCommand::StopPouringWater);
            userCommand = "stop pouring water";
            break;

        case '3':
            result = espressoMachine.command(EspressoMachineCommand::Boil);
            userCommand = "boil";
            break;

        case '4':
            result = espressoMachine.command(EspressoMachineCommand::MakeSteam);
            userCommand = "make steam";
            break;

        case '5':
            result = espressoMachine.command(EspressoMachineCommand::CoolDown);
            userCommand = "cool down";
            break;

        case '6':
            result = espressoMachine.command(EspressoMachineCommand::ToggleBoil);
            userCommand = "boil (simulating toggle)";
            break;

        case '7':
            result = espressoMachine.command(EspressoMachineCommand::ToggleMakeSteam);
            userCommand = "make steam (simulating toggle)";
            break;

        case '8':
            result = espressoMachine.command(EspressoMachineCommand::TogglePourWater);
            userCommand = "pour water (simulating toggle)";
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
        else
        {
            Serial.println("Unknown command");
        }
    }

    if (espressoMachine.getIsCommandChanged())
    {
        Serial.print("Espresso machine command changed to: ");
        printlnCommand(espressoMachine.getCommand());
    }

    espressoMachine.work();
}
