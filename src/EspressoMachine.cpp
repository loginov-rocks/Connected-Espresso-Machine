#include "EspressoMachine.h"

void EspressoMachine::setCommand(EspressoMachineCommand command)
{
    lastCommand = command;
    isCommandChanged = true;
}

EspressoMachine::EspressoMachine(int pumpPin, int boilerPin, int isBoilingPin, int isSteamPin, int togglePin, int _donePin) : pump(pumpPin),
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

boolean EspressoMachine::command(EspressoMachineCommand command)
{
    // Restrict internal commands usage programmatically.
    if (command == EspressoMachineCommand::ToggleBoil ||
        command == EspressoMachineCommand::ToggleMakeSteam ||
        command == EspressoMachineCommand::TogglePourWater)
    {
        return false;
    }

    // Restrict external commands if the toggle is not in the "Off" position.
    if (getToggleState() != ToggleState::Off)
    {
        return false;
    }

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

    // Operate boiler to achieve target state.
    boiler.work();

    // Operate "Done" pin to reflect the command state.
    isDone = (getBoilerTargetTemp() != BoilerTemp::Cold && getBoilerTemp() == getBoilerTargetTemp());
    digitalWrite(donePin, isDone);
}
