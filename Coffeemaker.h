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

        String _command;

        void setCommand(String);

    public:
        // Constructor
        Coffeemaker(int, int, int, int, int);

        // Commands
        boolean off();
        boolean pourWater();
        boolean stopPouringWater();
        boolean coolDown();
        boolean boil();
        boolean makeSteam();

        // Getters
        String          getCommand();
        boolean         getPumpState();
        boolean         getBoilerState();
        temp_t          getTemp();
        temp_t          getTargetTemp();
        toggle_state_t  getToggleState();

        // Update states
        void update();
};

#endif

