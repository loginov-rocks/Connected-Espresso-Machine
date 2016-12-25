/**
 * Arduino Coffemaker
 * Created by Danila Loginov, December 25, 2016
 * https://github.com/1oginov/Arduino-Coffeemaker
 */

#include "Relay.h"
#include "Boiler.h"

#define SERIAL_BAUDRATE         9600

#define PUMP_RELAY_PIN          2

#define BOILER_RELAY_PIN        3
#define BOILER_IS_BOILING_PIN   A0
#define BOILER_IS_STEAM_PIN     A1

Relay pump(PUMP_RELAY_PIN);
Boiler boiler(BOILER_RELAY_PIN, BOILER_IS_BOILING_PIN, BOILER_IS_STEAM_PIN);

boolean isSwitchedToSteam = false;
boolean isSwitchedToCold = false;

void setup() {
    Serial.begin(SERIAL_BAUDRATE);

    Serial.print("Initial pump state: ");
    Serial.println(pump.getState());

    Serial.print("Initial boiler state: ");
    Serial.println(boiler.getState());

    // Switch pump on for 10 seconds to test wiring and catch up pressure in boiler
    // Warning! Do not start to heat empty boiler
    Serial.println("Test pump for 10 seconds...");
    pump.on();
    delay(10000);
    pump.off();

    // Set target temp to BOILING and boiler will be switched on when first boiler.update() called
    Serial.println("Pump switched off, let's get some boiling water...");
    boiler.setTargetTemp(BOILING);
}

void loop() {
    // Boiler heated up to BOILING temp, we can switch it to STEAM and remeber this to prevent repeating
    if (!isSwitchedToSteam && boiler.getTemp() == BOILING) {
        Serial.println("Okey, we have boiling water, now we gonna raise the temperature up to steam...");
        boiler.setTargetTemp(STEAM);
        isSwitchedToSteam = true;
    }

    // Boiler heated up to STEAM temp, so we can switch it off and remember this to prevent repeating
    if (!isSwitchedToCold && boiler.getTemp() == STEAM) {
        Serial.println("Wow, so hot! Cool down, lil' boila...");
        boiler.setTargetTemp(COLD);
        isSwitchedToCold = true;
    }

    // Boiler cooled down, print a message and set flag to false to prevent repeating
    if (isSwitchedToCold && boiler.getTemp() == COLD) {
        Serial.println("Boiler has cooled down, but be careful! It's temperature just below boiling.");
        isSwitchedToCold = false;
    }

    boiler.update();
}

