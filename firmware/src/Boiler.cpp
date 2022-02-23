#include "Boiler.h"

Boiler::Boiler(int relayPin, int _isBoilingPin, int _isSteamPin) : Relay(relayPin)
{
    // Configure pins.
    isBoilingPin = _isBoilingPin;
    isSteamPin = _isSteamPin;

    pinMode(isBoilingPin, INPUT_PULLUP);
    pinMode(isSteamPin, INPUT_PULLUP);
}

boolean Boiler::getState()
{
    return Relay::getState();
}

BoilerTemp Boiler::getTemp()
{
    // The steam check goes first because the temp is hotter than when boiling.
    if (digitalRead(isSteamPin))
    {
        return BoilerTemp::Steam;
    }

    if (digitalRead(isBoilingPin))
    {
        return BoilerTemp::Boiling;
    }

    return BoilerTemp::Cold;
}

BoilerTemp Boiler::getTargetTemp()
{
    return targetTemp;
}

void Boiler::setTargetTemp(BoilerTemp _targetTemp)
{
    targetTemp = _targetTemp;
}

void Boiler::work()
{
    BoilerTemp temp = getTemp(),
               targetTemp = getTargetTemp();

    // Switch relay off if the target temp is achieved.
    if (targetTemp == BoilerTemp::Cold || temp == targetTemp || (temp == BoilerTemp::Steam && targetTemp == BoilerTemp::Boiling))
    {
        off();
    }
    else
    {
        on();
    }
}
