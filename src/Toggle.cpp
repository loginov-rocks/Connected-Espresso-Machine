#include "Toggle.h"

Toggle::Toggle(int pin)
{
    // Configure pin.
    _pin = pin;
    pinMode(_pin, INPUT);
}

boolean Toggle::_inBounds(int value, int target)
{
    return (value > target - _readingDeviation) && (value < target + _readingDeviation);
}

int Toggle::getOffReading()
{
    return _offReading;
}

int Toggle::getBoilReading()
{
    return _boilReading;
}

int Toggle::getMakeSteamReading()
{
    return _makeSteamReading;
}

int Toggle::getPourWaterReading()
{
    return _pourWaterReading;
}

int Toggle::getReadingDeviation()
{
    return _readingDeviation;
}

int Toggle::getDebounceTimeout()
{
    return _debounceTimeout;
}

toggle_state_t Toggle::getState()
{
    unsigned long _millis = millis();

    // Debounce by time.
    if (_lastReadingMillis + _debounceTimeout < _millis)
    {
        _lastReadingMillis = _millis;

        int reading = analogRead(_pin);
        toggle_state_t state;

        if (_inBounds(reading, getOffReading()))
        {
            state = OFF;
        }
        else if (_inBounds(reading, getBoilReading()))
        {
            state = BOIL;
        }
        else if (_inBounds(reading, getMakeSteamReading()))
        {
            state = MAKE_STEAM;
        }
        else if (_inBounds(reading, getPourWaterReading()))
        {
            state = POUR_WATER;
        }
        else
        {
            return _lastState;
        }

        // If the state has been updated, remember it and flag that it has been toggled.
        if (state != _lastState)
        {
            _lastState = state;
            _isToggled = true;
        }
    }

    return _lastState;
}

// One-time getter to know if the toggle has been toggled.
boolean Toggle::isToggled()
{
    if (!_isToggled)
    {
        return false;
    }

    _isToggled = false;

    return true;
}

void Toggle::setOffReading(int reading)
{
    _offReading = reading;
}

void Toggle::setBoilReading(int reading)
{
    _boilReading = reading;
}

void Toggle::setMakeSteamReading(int reading)
{
    _makeSteamReading = reading;
}

void Toggle::setPourWaterReading(int reading)
{
    _pourWaterReading = reading;
}

void Toggle::setReadingDeviation(int deviation)
{
    _readingDeviation = deviation;
}

void Toggle::setDebounceTimeout(int timeout)
{
    _debounceTimeout = timeout;
}
