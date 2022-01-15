#ifndef EspressoMachine_h
#define EspressoMachine_h

#include "Relay.h"
#include "Boiler.h"
#include "Toggle.h"

enum class EspressoMachineCommand
{
    Off,
    // External commands.
    PourWater,
    StopPouringWater,
    Boil,
    MakeSteam,
    CoolDown,
    MakeCoffee,
    // Internal commands.
    ToggleBoil,
    ToggleMakeSteam,
    TogglePourWater,
};

class EspressoMachine
{
private:
    Relay pump;
    Boiler boiler;
    Toggle toggle;
    int donePin;

    EspressoMachineCommand lastCommand = EspressoMachineCommand::Off;
    boolean isCommandChanged = false;
    boolean isDone = false;

    void setCommand(EspressoMachineCommand);
    void operateDone();

    // Helpers for the "Make Coffee" command.
    long makeCoffeeMillisLeft = 0;
    unsigned long makeCoffeePourWaterStartMillis = 0;
    void makeCoffeeOperate();

public:
    EspressoMachine(int, int, int, int, int, int);

    // Components getters.
    boolean getPumpState();
    boolean getBoilerState();
    BoilerTemp getBoilerTemp();
    BoilerTemp getBoilerTargetTemp();
    ToggleState getToggleState();

    // Getters.
    EspressoMachineCommand getCommand();
    boolean getIsCommandChanged();
    boolean getIsDone();
    long getMakeCoffeeMillisLeft();

    // Commands.
    boolean command(EspressoMachineCommand);
    boolean command(EspressoMachineCommand, int);

    // Processors.
    void work();
};

#endif
