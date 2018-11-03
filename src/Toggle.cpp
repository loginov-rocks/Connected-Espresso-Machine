#include "Toggle.h"

boolean inBounds(int value, int target, int deviation)
{
    return (value > target - deviation) && (value < target + deviation);
}

Toggle::Toggle(int pin)
{
    // Configure pin.
    _pin = pin;
    pinMode(_pin, INPUT);
}

toggle_state_t Toggle::getState()
{
    unsigned long _millis = millis();

    // Debounce by time.
    if (_lastReadingMillis + TOGGLE_DEBOUNCE_TIMEOUT < _millis)
    {
        _lastReadingMillis = _millis;

        int reading = analogRead(_pin);
        toggle_state_t state;

        if (inBounds(reading, TOGGLE_OFF_READING, TOGGLE_READING_DEVIATION))
        {
            state = OFF;
        }
        else if (inBounds(reading, TOGGLE_BOIL_READING, TOGGLE_READING_DEVIATION))
        {
            state = BOIL;
        }
        else if (inBounds(reading, TOGGLE_MAKE_STEAM_READING, TOGGLE_READING_DEVIATION))
        {
            state = MAKE_STEAM;
        }
        else if (inBounds(reading, TOGGLE_POUR_WATER_READING, TOGGLE_READING_DEVIATION))
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
