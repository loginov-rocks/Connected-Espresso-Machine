/**
 * Arduino Coffemaker — Boiler class header
 * Created by Danila Loginov, December 25, 2016
 * https://github.com/1oginov/Arduino-Coffeemaker
 */

#ifndef Boiler_h
#define Boiler_h

#include "Relay.h"

typedef enum {
    COLD,
    BOILING,
    STEAM
} temp_t;

class Boiler : private Relay {
    private:
        int _isBoilingPin;
        int _isSteamPin;
        temp_t _targetTemp;

    public:
        Boiler(int, int, int);
        boolean getState();
        temp_t getTemp();
        temp_t getTargetTemp();
        void setTargetTemp(temp_t);
        void update();
};

#endif

