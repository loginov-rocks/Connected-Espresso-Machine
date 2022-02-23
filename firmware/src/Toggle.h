#ifndef Toggle_h
#define Toggle_h

#include "Arduino.h"

enum class ToggleState
{
  Off,
  Boil,
  MakeSteam,
  PourWater
};

class Toggle
{
private:
  int pin;

  // States thresholds are analog readings from pins.
  int offReading = 0;
  int boilReading = 330;
  int makeSteamReading = 614;
  int pourWaterReading = 950;

  // Tolerance from the threshold.
  int readingDeviation = 100;

  // Milliseconds to prevent chatter.
  int debounceTimeout = 100;

  ToggleState lastState = ToggleState::Off;
  unsigned long lastReadingMillis = 0;
  boolean isToggled = false;

  boolean inBounds(int, int);

public:
  Toggle(int);

  // Getters.
  int getOffReading();
  int getBoilReading();
  int getMakeSteamReading();
  int getPourWaterReading();
  int getReadingDeviation();
  int getDebounceTimeout();
  ToggleState getState();
  boolean getIsToggled();

  // Setters.
  void setOffReading(int);
  void setBoilReading(int);
  void setMakeSteamReading(int);
  void setPourWaterReading(int);
  void setReadingDeviation(int);
  void setDebounceTimeout(int);
};

#endif
