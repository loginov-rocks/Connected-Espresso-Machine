#include "ArduinoOTA.h"
#include "EspressoMachine.h"
#include "WiFiManager.h"

#define SERIAL_BAUDRATE 9600

#define PUMP_RELAY_PIN D1
#define BOILER_RELAY_PIN D2
#define BOILER_IS_BOILING_PIN D5
#define BOILER_IS_STEAM_PIN D6
#define TOGGLE_PIN A0
#define DONE_PIN D7

EspressoMachine espressoMachine(PUMP_RELAY_PIN, BOILER_RELAY_PIN, BOILER_IS_BOILING_PIN, BOILER_IS_STEAM_PIN, TOGGLE_PIN, DONE_PIN);

void setupWiFi()
{
    Serial.println("Setup Wi-Fi...");

    WiFi.hostname("Connected-Espresso-Machine");

    WiFiManager wifiManager;
    wifiManager.autoConnect("Connected-Espresso-Machine");

    Serial.println("Wi-Fi setup was successful!");
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());
}

void setupOtaUpdates()
{
    Serial.println("Setup Over-the-Air Updates...");

    ArduinoOTA.setHostname("connected-espresso-machine");

    ArduinoOTA.onStart([]()
                       {
                           String type;

                           if (ArduinoOTA.getCommand() == U_FLASH)
                           {
                               type = "sketch";
                           }
                           else
                           {
                               // U_FS
                               type = "filesystem";
                           }

                           // NOTE: if updating FS this would be the place to unmount FS using FS.end()
                           Serial.println("Start updating " + type);
                       });

    ArduinoOTA.onEnd([]()
                     { Serial.println("\nEnd"); });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                          { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });

    ArduinoOTA.onError([](ota_error_t error)
                       {
                           Serial.printf("Error[%u]: ", error);

                           if (error == OTA_AUTH_ERROR)
                           {
                               Serial.println("Auth Failed");
                           }
                           else if (error == OTA_BEGIN_ERROR)
                           {
                               Serial.println("Begin Failed");
                           }
                           else if (error == OTA_CONNECT_ERROR)
                           {
                               Serial.println("Connect Failed");
                           }
                           else if (error == OTA_RECEIVE_ERROR)
                           {
                               Serial.println("Receive Failed");
                           }
                           else if (error == OTA_END_ERROR)
                           {
                               Serial.println("End Failed");
                           }
                       });

    ArduinoOTA.begin();

    Serial.println("Over-the-Air Updates setup was successful!");
}

void setup()
{
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println("Setup...");

    setupWiFi();
    setupOtaUpdates();

    Serial.println("Setup was successful!");
}

void loop()
{
    ArduinoOTA.handle();
    espressoMachine.work();
}
