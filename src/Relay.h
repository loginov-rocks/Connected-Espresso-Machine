#ifndef Relay_h
#define Relay_h

#include "Arduino.h"

class Relay
{
private:
  int pin;

public:
  Relay(int);

  // Getters.
  boolean getState();

  // Setters.
  void on();
  void off();
  void toggle();
};

#endif
