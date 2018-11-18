#ifndef Boiler_h
#define Boiler_h

#include "Relay.h"

typedef enum
{
  COLD,
  BOILING,
  STEAM
} boiler_temp_t;

class Boiler : private Relay
{
private:
  int _isBoilingPin;
  int _isSteamPin;
  boiler_temp_t _targetTemp = COLD;

public:
  Boiler(int, int, int);

  // Getters.
  boolean getState();
  boiler_temp_t getTemp();
  boiler_temp_t getTargetTemp();

  // Setters.
  void setTargetTemp(boiler_temp_t);

  void work();
};

#endif
