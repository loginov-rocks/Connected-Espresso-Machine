#include "RestEspressoMachine.h"

RestEspressoMachine::RestEspressoMachine(int pumpPin,
                                         int boilerPin,
                                         int isBoilingPin,
                                         int isSteamPin,
                                         int togglePin,
                                         int donePin,
                                         int _httpPort) : espressoMachine(pumpPin, boilerPin, isBoilingPin, isSteamPin, togglePin, donePin),
                                                          httpServer(_httpPort)
{
    // Remember HTTP port.
    httpPort = _httpPort;
}

void RestEspressoMachine::handleGetRoot()
{
    httpServer.send(418, "text/plain", "I'm a teapot\r\n");
}

void RestEspressoMachine::handleGetComponentsState()
{
    httpServer.send(200, "text/plain", "Components State\r\n");
}

void RestEspressoMachine::handleGetState()
{
    httpServer.send(200, "text/plain", "State\r\n");
}

void RestEspressoMachine::handlePostCommand()
{
    httpServer.send(204, "text/plain", "No Content\r\n");
}

void RestEspressoMachine::handleNotFound()
{
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += httpServer.uri();
    message += "\nMethod: ";
    message += (httpServer.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += httpServer.args();
    message += "\n";

    for (uint8_t i = 0; i < httpServer.args(); i++)
    {
        message += " " + httpServer.argName(i) + ": " + httpServer.arg(i) + "\n";
    }

    httpServer.send(404, "text/plain", message);
}

void RestEspressoMachine::setup()
{
    httpServer.on("/", std::bind(&RestEspressoMachine::handleGetRoot, this));
    httpServer.on("/components-state", std::bind(&RestEspressoMachine::handleGetComponentsState, this));
    httpServer.on("/state", std::bind(&RestEspressoMachine::handleGetState, this));
    httpServer.on("/command", std::bind(&RestEspressoMachine::handlePostCommand, this));
    httpServer.onNotFound(std::bind(&RestEspressoMachine::handleNotFound, this));

    httpServer.begin();
}

int RestEspressoMachine::getHttpPort()
{
    return httpPort;
}

void RestEspressoMachine::work()
{
    espressoMachine.work();
    httpServer.handleClient();
}
