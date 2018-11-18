#include "Boiler.h"

Boiler::Boiler(int relayPin, int isBoilingPin, int isSteamPin) : Relay(relayPin)
{
    // Configure pins.
    _isBoilingPin = isBoilingPin;
    _isSteamPin = isSteamPin;

    pinMode(_isBoilingPin, INPUT_PULLUP);
    pinMode(_isSteamPin, INPUT_PULLUP);
}

boolean Boiler::getState()
{
    return Relay::getState();
}

boiler_temp_t Boiler::getTemp()
{
    // Steam check goes first, because the temp is hotter than when boiling.
    if (digitalRead(_isSteamPin))
    {
        return STEAM;
    }

    if (digitalRead(_isBoilingPin))
    {
        return BOILING;
    }

    return COLD;
}

boiler_temp_t Boiler::getTargetTemp()
{
    return _targetTemp;
}

void Boiler::setTargetTemp(boiler_temp_t targetTemp)
{
    _targetTemp = targetTemp;
}

void Boiler::work()
{
    boiler_temp_t temp = getTemp(),
                  targetTemp = getTargetTemp();

    // Switch relay off if the target temp is achieved.
    if (targetTemp == COLD || temp == targetTemp || (temp == STEAM && targetTemp == BOILING))
    {
        off();
    }
    else
    {
        on();
    }
}
