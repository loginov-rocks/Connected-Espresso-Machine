#include "Relay.h"

Relay::Relay(int _pin)
{
    // Configure pin.
    pin = _pin;
    pinMode(pin, OUTPUT);

    // Off on init.
    off();
}

boolean Relay::getState()
{
    return digitalRead(pin);
}

void Relay::on()
{
    digitalWrite(pin, HIGH);
}

void Relay::off()
{
    digitalWrite(pin, LOW);
}

void Relay::toggle()
{
    digitalWrite(pin, !getState());
}
