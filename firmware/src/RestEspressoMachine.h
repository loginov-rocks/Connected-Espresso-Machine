#ifndef RestEspressoMachine_h
#define RestEspressoMachine_h

#include "ESP8266WebServer.h"
#include "EspressoMachine.h"

class RestEspressoMachine
{
private:
    // Helpers.
    static String boilerTempToString(BoilerTemp);
    static String toggleStateToString(ToggleState);
    static String commandToString(EspressoMachineCommand);
    static boolean isValidCommandString(String);
    static EspressoMachineCommand stringToCommand(String);

    EspressoMachine espressoMachine;
    ESP8266WebServer httpServer;
    int httpPort;
    String scriptsUrl;
    String stylesUrl;

    // HTTP Handlers.
    void handleGetRoot();
    void handleGetState();
    void handlePostCommand();
    void handleNotFound();

public:
    RestEspressoMachine(int, int, int, int, int, int, int, String, String);

    // Setup.
    void setup();

    // Getters.
    int getHttpPort();

    // Processors.
    void work();
};

#endif
