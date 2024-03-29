#include "ArduinoOTA.h"
#include "RestEspressoMachine.h"
#include "WiFiManager.h"

#define SERIAL_BAUDRATE 9600

#define WIFI_HOSTNAME "Connected-Espresso-Machine"
#define WIFI_ACCESS_POINT_SSID "Connected-Espresso-Machine"

#define OTA_UPDATES_HOSTNAME "connected-espresso-machine"

#define PUMP_RELAY_PIN D1
#define BOILER_RELAY_PIN D2
#define BOILER_IS_BOILING_PIN D5
#define BOILER_IS_STEAM_PIN D6
#define TOGGLE_PIN A0
#define DONE_PIN D7
#define HTTP_PORT 80
#define SCRIPTS_URL "https://connected-espresso-machine-web-assets-bucket.s3.us-east-1.amazonaws.com/scripts.js"
#define STYLES_URL "https://connected-espresso-machine-web-assets-bucket.s3.us-east-1.amazonaws.com/styles.css"

RestEspressoMachine restEspressoMachine(PUMP_RELAY_PIN, BOILER_RELAY_PIN, BOILER_IS_BOILING_PIN, BOILER_IS_STEAM_PIN, TOGGLE_PIN, DONE_PIN, HTTP_PORT, SCRIPTS_URL, STYLES_URL);

/**
 * @see https://github.com/tzapu/WiFiManager/blob/master/examples/Basic/Basic.ino
 */
void setupWiFi()
{
    Serial.println("Setup Wi-Fi...");

    WiFi.hostname(WIFI_HOSTNAME);

    WiFiManager wifiManager;
    wifiManager.autoConnect(WIFI_ACCESS_POINT_SSID);

    Serial.println("Wi-Fi setup was successful!");
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());
}

/**
 * @see https://github.com/esp8266/Arduino/blob/master/libraries/ArduinoOTA/examples/BasicOTA/BasicOTA.ino
 */
void setupOtaUpdates()
{
    Serial.println("Setup Over-the-Air Updates...");

    ArduinoOTA.setHostname(OTA_UPDATES_HOSTNAME);

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

void setupRestEspressoMachine()
{
    Serial.println("Setup Rest Espresso Machine...");

    restEspressoMachine.setup();

    Serial.println("Rest Espresso Machine setup was successful!");
    Serial.print("HTTP port: ");
    Serial.println(restEspressoMachine.getHttpPort());
}

void setup()
{
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println("Setup...");

    setupWiFi();
    setupOtaUpdates();
    setupRestEspressoMachine();

    Serial.println("Setup was successful!");
}

void loop()
{
    ArduinoOTA.handle();
    restEspressoMachine.work();
}
