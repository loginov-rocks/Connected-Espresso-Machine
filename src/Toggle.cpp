#include "Toggle.h"

boolean Toggle::inBounds(int value, int target)
{
    return (value > target - readingDeviation) && (value < target + readingDeviation);
}

Toggle::Toggle(int _pin)
{
    // Configure pin.
    pin = pin;
    pinMode(pin, INPUT);
}

int Toggle::getOffReading()
{
    return offReading;
}

int Toggle::getBoilReading()
{
    return boilReading;
}

int Toggle::getMakeSteamReading()
{
    return makeSteamReading;
}

int Toggle::getPourWaterReading()
{
    return pourWaterReading;
}

int Toggle::getReadingDeviation()
{
    return readingDeviation;
}

int Toggle::getDebounceTimeout()
{
    return debounceTimeout;
}

ToggleState Toggle::getState()
{
    unsigned long _millis = millis();

    // Debounce by time.
    if (lastReadingMillis + debounceTimeout < _millis)
    {
        lastReadingMillis = _millis;

        int reading = analogRead(pin);
        ToggleState state;

        if (inBounds(reading, getOffReading()))
        {
            state = ToggleState::Off;
        }
        else if (inBounds(reading, getBoilReading()))
        {
            state = ToggleState::Boil;
        }
        else if (inBounds(reading, getMakeSteamReading()))
        {
            state = ToggleState::MakeSteam;
        }
        else if (inBounds(reading, getPourWaterReading()))
        {
            state = ToggleState::PourWater;
        }
        else
        {
            return lastState;
        }

        // If the state has been updated, remember it and flag that it has been toggled.
        if (state != lastState)
        {
            lastState = state;
            isToggled = true;
        }
    }

    return lastState;
}

// One-time getter to know if the toggle has been toggled.
boolean Toggle::getIsToggled()
{
    if (!isToggled)
    {
        return false;
    }

    isToggled = false;

    return true;
}

void Toggle::setOffReading(int reading)
{
    offReading = reading;
}

void Toggle::setBoilReading(int reading)
{
    boilReading = reading;
}

void Toggle::setMakeSteamReading(int reading)
{
    makeSteamReading = reading;
}

void Toggle::setPourWaterReading(int reading)
{
    pourWaterReading = reading;
}

void Toggle::setReadingDeviation(int deviation)
{
    readingDeviation = deviation;
}

void Toggle::setDebounceTimeout(int timeout)
{
    debounceTimeout = timeout;
}
