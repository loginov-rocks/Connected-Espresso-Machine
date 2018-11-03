#ifndef Boiler_h
#define Boiler_h

#include "Relay.h"

typedef enum
{
  COLD,
  BOILING,
  STEAM
} temp_t;

class Boiler : private Relay
{
private:
  int _isBoilingPin;
  int _isSteamPin;
  temp_t _targetTemp = COLD;

public:
  Boiler(int, int, int);
  boolean getState();
  temp_t getTemp();
  temp_t getTargetTemp();
  void setTargetTemp(temp_t);
  void work();
};

#endif
