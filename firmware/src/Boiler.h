#ifndef Boiler_h
#define Boiler_h

#include "Relay.h"

enum class BoilerTemp
{
  Cold,
  Boiling,
  Steam
};

class Boiler : private Relay
{
private:
  int isBoilingPin;
  int isSteamPin;

  BoilerTemp targetTemp = BoilerTemp::Cold;

public:
  Boiler(int, int, int);

  // Getters.
  boolean getState();
  BoilerTemp getTemp();
  BoilerTemp getTargetTemp();

  // Setters.
  void setTargetTemp(BoilerTemp);

  // Processors.
  void work();
};

#endif
