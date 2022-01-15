#include "EspressoMachine.h"

void EspressoMachine::setCommand(EspressoMachineCommand command)
{
    lastCommand = command;
    isCommandChanged = true;
}

void EspressoMachine::operateDone()
{
    isDone = false;

    if (getCommand() == EspressoMachineCommand::MakeCoffee)
    {
        if (makeCoffeeMillisLeft <= 0)
        {
            isDone = true;
        }
    }
    else if (getBoilerTargetTemp() != BoilerTemp::Cold && getBoilerTemp() == getBoilerTargetTemp())
    {
        isDone = true;
    }

    digitalWrite(donePin, isDone);
}

void EspressoMachine::makeCoffeeOperate()
{
    // Safety condition: switch components off if no milliseconds left to make coffee.
    if (makeCoffeeMillisLeft <= 0)
    {
        pump.off();
        boiler.setTargetTemp(BoilerTemp::Cold);

        return;
    }

    // Water pouring has not started yet.
    if (makeCoffeePourWaterStartMillis <= 0)
    {
        boiler.setTargetTemp(BoilerTemp::Boiling);

        // A suitable temperature is achieved, start to pour water and remember when.
        if (getBoilerTemp() == BoilerTemp::Boiling)
        {
            pump.on();
            makeCoffeePourWaterStartMillis = millis();
        }

        return;
    }

    unsigned long _millis = millis();

    // There are milliseconds left to make coffee, water pouring started, but the target time has not been achieved yet.
    if (makeCoffeePourWaterStartMillis + makeCoffeeMillisLeft >= _millis)
    {
        // Temperature is not suitable, pause water pouring and remember milliseconds left.
        if (getBoilerTemp() != BoilerTemp::Boiling)
        {
            pump.off();
            makeCoffeeMillisLeft -= _millis - makeCoffeePourWaterStartMillis;
            makeCoffeePourWaterStartMillis = 0;
        }

        return;
    }

    // Target time to make coffee achieved.
    pump.off();
    boiler.setTargetTemp(BoilerTemp::Cold);
    makeCoffeeMillisLeft = 0;
    makeCoffeePourWaterStartMillis = 0;
}

EspressoMachine::EspressoMachine(int pumpPin,
                                 int boilerPin,
                                 int isBoilingPin,
                                 int isSteamPin,
                                 int togglePin,
                                 int _donePin) : pump(pumpPin),
                                                 boiler(boilerPin, isBoilingPin, isSteamPin),
                                                 toggle(togglePin)
{
    // Configure "Done" pin.
    donePin = _donePin;
    pinMode(donePin, OUTPUT);
    digitalWrite(donePin, LOW);
}

boolean EspressoMachine::getPumpState()
{
    return pump.getState();
}

boolean EspressoMachine::getBoilerState()
{
    return boiler.getState();
}

BoilerTemp EspressoMachine::getBoilerTemp()
{
    return boiler.getTemp();
}

BoilerTemp EspressoMachine::getBoilerTargetTemp()
{
    return boiler.getTargetTemp();
}

ToggleState EspressoMachine::getToggleState()
{
    return toggle.getState();
}

EspressoMachineCommand EspressoMachine::getCommand()
{
    return lastCommand;
}

// One-time getter to know if the command has been changed.
boolean EspressoMachine::getIsCommandChanged()
{
    if (!isCommandChanged)
    {
        return false;
    }

    isCommandChanged = false;

    return true;
}

boolean EspressoMachine::getIsDone()
{
    return isDone;
}

long EspressoMachine::getMakeCoffeeMillisLeft()
{
    return makeCoffeeMillisLeft;
}

boolean EspressoMachine::command(EspressoMachineCommand command)
{
    // Deny the "Make Coffee" command, use the overloaded method to pass arguments.
    if (command == EspressoMachineCommand::MakeCoffee)
    {
        return false;
    }

    // Deny internal commands usage programmatically.
    if (command == EspressoMachineCommand::ToggleBoil ||
        command == EspressoMachineCommand::ToggleMakeSteam ||
        command == EspressoMachineCommand::TogglePourWater)
    {
        return false;
    }

    // Deny external commands if the toggle is not in the "Off" position.
    if (getToggleState() != ToggleState::Off)
    {
        return false;
    }

    // Drop helpers for the "Make Coffee" command.
    makeCoffeeMillisLeft = 0;
    makeCoffeePourWaterStartMillis = 0;

    setCommand(command);

    return true;
}

// Overload for the "Make Coffee" command to pass arguments.
boolean EspressoMachine::command(EspressoMachineCommand command, int seconds)
{
    // Deny other commands.
    if (command != EspressoMachineCommand::MakeCoffee)
    {
        return false;
    }

    // Seconds should be greater than null.
    if (seconds <= 0)
    {
        return false;
    }

    // Deny external commands if the toggle is not in the "Off" position.
    if (getToggleState() != ToggleState::Off)
    {
        return false;
    }

    // Set helpers for the "Make Coffee" command.
    makeCoffeeMillisLeft = (long)seconds * 1000;
    makeCoffeePourWaterStartMillis = 0;

    setCommand(command);

    return true;
}

void EspressoMachine::work()
{
    // Get toggle state before checking whether it was toggled to have the flag up to date.
    ToggleState toggleState = getToggleState();

    // Set the command if the toggle has been toggled.
    if (toggle.getIsToggled())
    {
        switch (toggleState)
        {
        case ToggleState::Boil:
            setCommand(EspressoMachineCommand::ToggleBoil);
            break;

        case ToggleState::MakeSteam:
            setCommand(EspressoMachineCommand::ToggleMakeSteam);
            break;

        case ToggleState::PourWater:
            setCommand(EspressoMachineCommand::TogglePourWater);
            break;

        // Off by default.
        default:
            setCommand(EspressoMachineCommand::Off);
            break;
        }
    }

    // Operate components based on the command value.
    switch (lastCommand)
    {
    // External commands.
    case EspressoMachineCommand::PourWater:
        pump.on();
        break;

    case EspressoMachineCommand::StopPouringWater:
        pump.off();
        break;

    case EspressoMachineCommand::Boil:
        boiler.setTargetTemp(BoilerTemp::Boiling);
        break;

    case EspressoMachineCommand::MakeSteam:
        boiler.setTargetTemp(BoilerTemp::Steam);
        break;

    case EspressoMachineCommand::CoolDown:
        boiler.setTargetTemp(BoilerTemp::Cold);
        break;

    case EspressoMachineCommand::MakeCoffee:
        // Encapsulate related operations in a separate function.
        makeCoffeeOperate();
        break;

    // Internal commands.
    case EspressoMachineCommand::ToggleBoil:
        pump.off();
        boiler.setTargetTemp(BoilerTemp::Boiling);
        break;

    case EspressoMachineCommand::ToggleMakeSteam:
        pump.off();
        boiler.setTargetTemp(BoilerTemp::Steam);
        break;

    case EspressoMachineCommand::TogglePourWater:
        pump.on();
        boiler.setTargetTemp(BoilerTemp::Cold);
        break;

    // Off by default.
    default:
        pump.off();
        boiler.setTargetTemp(BoilerTemp::Cold);
        break;
    }

    // Operate boiler to achieve the target state.
    boiler.work();

    // Operate the "Done" pin to reflect the command state.
    operateDone();
}
