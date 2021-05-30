#ifndef Coffeemaker_h
#define Coffeemaker_h

#include "Relay.h"
#include "Boiler.h"
#include "Toggle.h"

class Coffeemaker
{
private:
    Relay pump;
    Boiler boiler;
    Toggle toggle;
    int donePin;

    String command;
    boolean isDone;
    long millisLeftToMakeCoffee;
    unsigned long pourWaterStartMillis;

    void setCommand(String);

public:
    Coffeemaker(int, int, int, int, int, int);

    // Commands.
    boolean off();
    boolean pourWater();
    boolean stopPouringWater();
    boolean coolDown();
    boolean boil();
    boolean makeSteam();
    boolean makeCoffee(int);

    // Getters.
    String getCommand();
    boolean getPumpState();
    boolean getBoilerState();
    BoilerTemp getTemp();
    BoilerTemp getTargetTemp();
    ToggleState getToggleState();
    long getMillisLeftToMakeCoffee();
    boolean getIsDone();

    // Processors.
    void work();
};

#endif
