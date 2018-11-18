#ifndef Toggle_h
#define Toggle_h

#include "Arduino.h"

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

  // States thresholds are analog readings from pin.
  int _offReading = 0;
  int _boilReading = 330;
  int _makeSteamReading = 614;
  int _pourWaterReading = 950;

  // Tolerance from threshold.
  int _readingDeviation = 50;

  // Milliseconds to prevent chatter.
  int _debounceTimeout = 100;

  boolean _inBounds(int, int);

  toggle_state_t _lastState = OFF;
  unsigned long _lastReadingMillis = 0;
  boolean _isToggled = false;

public:
  Toggle(int);

  // Getters.
  int getOffReading();
  int getBoilReading();
  int getMakeSteamReading();
  int getPourWaterReading();
  int getReadingDeviation();
  int getDebounceTimeout();

  // Setters.
  void setOffReading(int);
  void setBoilReading(int);
  void setMakeSteamReading(int);
  void setPourWaterReading(int);
  void setReadingDeviation(int);
  void setDebounceTimeout(int);

  toggle_state_t getState();
  boolean isToggled();
};

#endif
