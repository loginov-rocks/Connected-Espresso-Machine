/**
 * Arduino Coffemaker
 * Created by Danila Loginov, December 25, 2016
 * https://github.com/1oginov/Arduino-Coffeemaker
 */

#include "Relay.h"

#define SERIAL_BAUDRATE 9600
#define RELAY_PIN       2

Relay relay(RELAY_PIN);

void setup() {
    Serial.begin(SERIAL_BAUDRATE);
    Serial.print("Relay state: ");
    Serial.println(relay.getState());
}

void loop() {
    relay.toggle();
    Serial.print("Relay state: ");
    Serial.println(relay.getState());
    delay(2000);
}

