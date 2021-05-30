#include "Coffeemaker.h"

void Coffeemaker::setCommand(String _command)
{
    command = _command;

    // Reset metrics related to making a coffee command.
    if (command != "makeCoffee")
    {
        millisLeftToMakeCoffee = 0;
        pourWaterStartMillis = 0;
    }
}

Coffeemaker::Coffeemaker(int pumpPin, int boilerPin, int isBoilingPin, int isSteamPin, int togglePin, int _donePin) : pump(pumpPin),
                                                                                                                      boiler(boilerPin, isBoilingPin, isSteamPin),
                                                                                                                      toggle(togglePin)
{

    donePin = _donePin;
    pinMode(donePin, OUTPUT);
    digitalWrite(donePin, LOW);

    setCommand("off");
}

boolean Coffeemaker::off()
{
    if (getToggleState() != ToggleState::Off)
    {
        return false;
    }

    pump.off();
    boiler.setTargetTemp(BoilerTemp::Cold);

    setCommand(__FUNCTION__);
    return true;
}

boolean Coffeemaker::pourWater()
{
    if (getToggleState() != ToggleState::Off)
    {
        return false;
    }

    pump.on();

    setCommand(__FUNCTION__);
    return true;
}

boolean Coffeemaker::stopPouringWater()
{
    if (getToggleState() != ToggleState::Off)
    {
        return false;
    }

    pump.off();

    setCommand(__FUNCTION__);
    return true;
}

boolean Coffeemaker::coolDown()
{
    if (getToggleState() != ToggleState::Off)
    {
        return false;
    }

    boiler.setTargetTemp(BoilerTemp::Cold);

    setCommand(__FUNCTION__);
    return true;
}

boolean Coffeemaker::boil()
{
    if (getToggleState() != ToggleState::Off)
    {
        return false;
    }

    boiler.setTargetTemp(BoilerTemp::Boiling);

    setCommand(__FUNCTION__);
    return true;
}

boolean Coffeemaker::makeSteam()
{
    if (getToggleState() != ToggleState::Off)
    {
        return false;
    }

    boiler.setTargetTemp(BoilerTemp::Steam);

    setCommand(__FUNCTION__);
    return true;
}

boolean Coffeemaker::makeCoffee(int seconds)
{
    if (seconds <= 0 || getToggleState() != ToggleState::Off)
    {
        return false;
    }

    pump.off();
    boiler.setTargetTemp(BoilerTemp::Boiling);

    millisLeftToMakeCoffee = (long)seconds * 1000;
    pourWaterStartMillis = 0;

    setCommand(__FUNCTION__);
    return true;
}

String Coffeemaker::getCommand()
{
    return command;
}

boolean Coffeemaker::getPumpState()
{
    return pump.getState();
}

boolean Coffeemaker::getBoilerState()
{
    return boiler.getState();
}

BoilerTemp Coffeemaker::getTemp()
{
    return boiler.getTemp();
}

BoilerTemp Coffeemaker::getTargetTemp()
{
    return boiler.getTargetTemp();
}

ToggleState Coffeemaker::getToggleState()
{
    return toggle.getState();
}

long Coffeemaker::getMillisLeftToMakeCoffee()
{
    if (pourWaterStartMillis > 0)
    {
        return millisLeftToMakeCoffee - millis() + pourWaterStartMillis;
    }

    return millisLeftToMakeCoffee;
}

boolean Coffeemaker::getIsDone()
{
    if (getCommand() == "makeCoffee" && isDone)
    {
        off();
        isDone = false;
        return true;
    }

    return isDone;
}

void Coffeemaker::work()
{

    // Get toggle state and execute a command if it has been toggled.
    ToggleState toggleState = getToggleState();

    if (toggle.getIsToggled())
    {
        switch (toggleState)
        {
        case ToggleState::Boil:
            pump.off();
            boiler.setTargetTemp(BoilerTemp::Boiling);
            setCommand("boil");
            break;

        case ToggleState::MakeSteam:
            pump.off();
            boiler.setTargetTemp(BoilerTemp::Steam);
            setCommand("makeSteam");
            break;

        case ToggleState::PourWater:
            pump.on();
            boiler.setTargetTemp(BoilerTemp::Cold);
            setCommand("pourWater");
            break;

        default:
            off();
            break;
        }
    }

    // Update boiler state, toggle heating depending on demands.
    boiler.work();

    // Making coffee.
    if (millisLeftToMakeCoffee > 0)
    {

        // Water pouring has been already started.
        if (pourWaterStartMillis > 0)
        {

            // The time to make coffee is over.
            if (pourWaterStartMillis + millisLeftToMakeCoffee < millis())
            {
                pump.off();
                boiler.setTargetTemp(BoilerTemp::Cold);
                millisLeftToMakeCoffee = 0;
                pourWaterStartMillis = 0;
            }

            // Temperature is not suitable, suspend making coffee and update left time.
            else if (getTemp() != BoilerTemp::Boiling)
            {
                pump.off();
                millisLeftToMakeCoffee -= millis() - pourWaterStartMillis;
                pourWaterStartMillis = 0;
            }
        }

        // Suitable temperature, start to pour water.
        else if (getTemp() == BoilerTemp::Boiling)
        {
            pump.on();
            pourWaterStartMillis = millis();
        }
    }

    // Indicate commands execution by done LED.
    isDone = false;
    if (getCommand() == "makeCoffee")
    {
        if (millisLeftToMakeCoffee == 0)
        {
            isDone = true;
        }
    }
    else if (getTargetTemp() != BoilerTemp::Cold && getTemp() == getTargetTemp())
    {
        isDone = true;
    }
    digitalWrite(donePin, isDone);
}
