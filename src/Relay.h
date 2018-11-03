#ifndef Relay_h
#define Relay_h

#include "Arduino.h"

class Relay
{
private:
  int _pin;

public:
  Relay(int);
  boolean getState();
  void on();
  void off();
  void toggle();
};

#endif
