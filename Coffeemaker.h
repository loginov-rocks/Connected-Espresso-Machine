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
        Relay _pump;
        Boiler _boiler;
        Toggle _toggle;

    public:
        Coffeemaker(int, int, int, int, int);

        boolean getPumpState();

        boolean getBoilerState();
        temp_t getTemp();
        temp_t getTargetTemp();

        toggle_state_t getToggleState();

        void update();
};

#endif

