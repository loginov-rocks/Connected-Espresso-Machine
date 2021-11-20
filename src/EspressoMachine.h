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

    // Commands.
    boolean command(EspressoMachineCommand);

    // Processors.
    void work();
};

#endif
