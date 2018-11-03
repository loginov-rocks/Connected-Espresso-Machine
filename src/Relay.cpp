#include "Relay.h"

Relay::Relay(int pin)
{
    // Configure pin.
    _pin = pin;
    pinMode(_pin, OUTPUT);

    // Off on init.
    off();
}

boolean Relay::getState()
{
    return digitalRead(_pin);
}

void Relay::on()
{
    digitalWrite(_pin, HIGH);
}

void Relay::off()
{
    digitalWrite(_pin, LOW);
}

void Relay::toggle()
{
    digitalWrite(_pin, !getState());
}
