/**
 * Arduino Coffemaker — Coffeemaker class header
 * Created by Danila Loginov, December 26, 2016
 * https://github.com/1oginov/Arduino-Coffeemaker
 */

#ifndef Coffeemaker_h
#define Coffeemaker_h

#include "Relay.h"
#include "Boiler.h"
#include "Toggle.h"

class Coffeemaker {
    private:
        Relay   _pump;
        Boiler  _boiler;
        Toggle  _toggle;
        int     _donePin;

        String          _command;
        boolean         _isDone;
        long            _millisLeftToMakeCoffee;
        unsigned long   _pourWaterStartMillis;

        void setCommand(String);

    public:
        // Constructor
        Coffeemaker(int, int, int, int, int, int);

        // Commands
        boolean off();
        boolean pourWater();
        boolean stopPouringWater();
        boolean coolDown();
        boolean boil();
        boolean makeSteam();
        boolean makeCoffee(int);

        // Getters
        String          getCommand();
        boolean         getPumpState();
        boolean         getBoilerState();
        temp_t          getTemp();
        temp_t          getTargetTemp();
        toggle_state_t  getToggleState();
        long            getMillisLeftToMakeCoffee();
        boolean         isDone();

        // Update states
        void update();
};

#endif

