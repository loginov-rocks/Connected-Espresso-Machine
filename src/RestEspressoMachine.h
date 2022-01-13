#ifndef RestEspressoMachine_h
#define RestEspressoMachine_h

#include "ESP8266WebServer.h"
#include "EspressoMachine.h"

class RestEspressoMachine
{
private:
    EspressoMachine espressoMachine;
    ESP8266WebServer httpServer;
    int httpPort;

    // HTTP Handlers.
    void handleGetRoot();
    void handleGetComponentsState();
    void handleGetState();
    void handlePostCommand();
    void handleNotFound();

public:
    RestEspressoMachine(int, int, int, int, int, int, int);

    // Setup.
    void setup();

    // Getters.
    int getHttpPort();

    // Processors.
    void work();
};

#endif
