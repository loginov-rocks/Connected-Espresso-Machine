#ifndef Toggle_h
#define Toggle_h

#include "Arduino.h"

// States thresholds are analog readings from pin.
#define TOGGLE_OFF_READING 0
#define TOGGLE_BOIL_READING 330
#define TOGGLE_MAKE_STEAM_READING 614
#define TOGGLE_POUR_WATER_READING 950

// Tolerance from threshold.
#define TOGGLE_READING_DEVIATION 50

// Milliseconds to prevent chatter.
#define TOGGLE_DEBOUNCE_TIMEOUT 100

typedef enum
{
  OFF,
  BOIL,
  MAKE_STEAM,
  POUR_WATER
} toggle_state_t;

class Toggle
{
private:
  int _pin;
  toggle_state_t _lastState = OFF;
  unsigned long _lastReadingMillis = 0;
  boolean _isToggled = false;

public:
  Toggle(int);
  toggle_state_t getState();
  boolean isToggled();
};

#endif
