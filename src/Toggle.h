/**
 * Arduino Coffemaker — Toggle class header
 * Created by Danila Loginov, December 26, 2016
 * https://github.com/1oginov/Arduino-Coffeemaker
 */

#ifndef Toggle_h
#define Toggle_h

#include "Arduino.h"

#define TOGGLE_OFF_READING          0
#define TOGGLE_BOIL_READING         330
#define TOGGLE_MAKE_STEAM_READING   614
#define TOGGLE_POUR_WATER_READING   950
#define TOGGLE_READING_DEVIATION    50
#define TOGGLE_DEBOUNCE_TIMEOUT     100

typedef enum {
    OFF,
    BOIL,
    MAKE_STEAM,
    POUR_WATER
} toggle_state_t;

class Toggle {
    private:
        int _pin;
        toggle_state_t _lastState;
        unsigned long _lastReadingMillis;
        boolean _toggled;

    public:
        Toggle(int);
        toggle_state_t getState();
        boolean isToggled();
};

#endif

