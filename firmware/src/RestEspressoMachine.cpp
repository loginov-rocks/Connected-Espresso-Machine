#include "ArduinoJson.h"
#include "RestEspressoMachine.h"

String RestEspressoMachine::boilerTempToString(BoilerTemp boilerTemp)
{
    switch (boilerTemp)
    {
    case BoilerTemp::Cold:
        return "cold";
    case BoilerTemp::Boiling:
        return "boiling";
    case BoilerTemp::Steam:
        return "steam";
    default:
        return "UNKNOWN";
    }
}

String RestEspressoMachine::toggleStateToString(ToggleState toggleState)
{
    switch (toggleState)
    {
    case ToggleState::Off:
        return "off";
    case ToggleState::Boil:
        return "boil";
    case ToggleState::MakeSteam:
        return "makeSteam";
    case ToggleState::PourWater:
        return "pourWater";
    default:
        return "UNKNOWN";
    }
}

String RestEspressoMachine::commandToString(EspressoMachineCommand command)
{
    switch (command)
    {
    case EspressoMachineCommand::Off:
        return "off";
    // External commands.
    case EspressoMachineCommand::PourWater:
        return "pourWater";
    case EspressoMachineCommand::StopPouringWater:
        return "stopPouringWater";
    case EspressoMachineCommand::Boil:
        return "boil";
    case EspressoMachineCommand::MakeSteam:
        return "makeSteam";
    case EspressoMachineCommand::CoolDown:
        return "coolDown";
    case EspressoMachineCommand::MakeCoffee:
        return "makeCoffee";
    // Internal commands.
    case EspressoMachineCommand::ToggleBoil:
        return "toggleBoil";
    case EspressoMachineCommand::ToggleMakeSteam:
        return "toggleMakeSteam";
    case EspressoMachineCommand::TogglePourWater:
        return "togglePourWater";
    default:
        return "UNKNOWN";
    }
}

// TODO: Optimize.
boolean RestEspressoMachine::isValidCommandString(String string)
{
    return string.equals("off") ||
           // External commands.
           string.equals("pourWater") ||
           string.equals("stopPouringWater") ||
           string.equals("boil") ||
           string.equals("makeSteam") ||
           string.equals("coolDown") ||
           string.equals("makeCoffee") ||
           // Internal commands.
           string.equals("toggleBoil") ||
           string.equals("toggleMakeSteam") ||
           string.equals("togglePourWater");
}

// TODO: Optimize.
EspressoMachineCommand RestEspressoMachine::stringToCommand(String string)
{
    // External commands.
    if (string.equals("pourWater"))
    {
        return EspressoMachineCommand::PourWater;
    }
    if (string.equals("stopPouringWater"))
    {
        return EspressoMachineCommand::StopPouringWater;
    }
    if (string.equals("boil"))
    {
        return EspressoMachineCommand::Boil;
    }
    if (string.equals("makeSteam"))
    {
        return EspressoMachineCommand::MakeSteam;
    }
    if (string.equals("coolDown"))
    {
        return EspressoMachineCommand::CoolDown;
    }
    if (string.equals("makeCoffee"))
    {
        return EspressoMachineCommand::MakeCoffee;
    }

    // Internal commands.
    if (string.equals("toggleBoil"))
    {
        return EspressoMachineCommand::ToggleBoil;
    }
    if (string.equals("toggleMakeSteam"))
    {
        return EspressoMachineCommand::ToggleMakeSteam;
    }
    if (string.equals("togglePourWater"))
    {
        return EspressoMachineCommand::TogglePourWater;
    }

    return EspressoMachineCommand::Off;
}

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
    httpServer.send(418, "text/plain", "I'm a teapot");
}

void RestEspressoMachine::handleGetState()
{
    // @see https://arduinojson.org/v6/how-to/determine-the-capacity-of-the-jsondocument/
    DynamicJsonDocument state(256);
    state["command"] = RestEspressoMachine::commandToString(espressoMachine.getCommand());
    state["isCommandChanged"] = espressoMachine.getIsCommandChanged();
    state["isDone"] = espressoMachine.getIsDone();
    state["makeCoffeeMillisLeft"] = espressoMachine.getMakeCoffeeMillisLeft();

    JsonObject componentsState = state.createNestedObject("components");
    componentsState["pump"] = espressoMachine.getPumpState();
    componentsState["boiler"] = espressoMachine.getBoilerState();
    componentsState["boilerTemp"] = RestEspressoMachine::boilerTempToString(espressoMachine.getBoilerTemp());
    componentsState["boilerTargetTemp"] = RestEspressoMachine::boilerTempToString(espressoMachine.getBoilerTargetTemp());
    componentsState["toggleState"] = RestEspressoMachine::toggleStateToString(espressoMachine.getToggleState());

    String response;
    serializeJson(state, response);

    httpServer.send(200, "application/json", response);
}

void RestEspressoMachine::handlePostCommand()
{
    String commandString = httpServer.arg("command");

    if (!RestEspressoMachine::isValidCommandString(commandString))
    {
        httpServer.send(400, "text/plain", "Bad Request");
        return;
    }

    EspressoMachineCommand command = RestEspressoMachine::stringToCommand(commandString);

    // Deny internal commands usage programmatically.
    if (command == EspressoMachineCommand::ToggleBoil ||
        command == EspressoMachineCommand::ToggleMakeSteam ||
        command == EspressoMachineCommand::TogglePourWater)
    {
        httpServer.send(400, "text/plain", "Bad Request");
        return;
    }

    // Deny external commands if the toggle is not in the "Off" position.
    if (espressoMachine.getToggleState() != ToggleState::Off)
    {
        httpServer.send(409, "text/plain", "Conflict");
        return;
    }

    // Handle the "Make Coffee" command separately to use the overloaded method.
    if (command == EspressoMachineCommand::MakeCoffee)
    {
        int seconds = httpServer.arg("seconds").toInt();

        // Seconds should be greater than null.
        if (seconds <= 0)
        {
            httpServer.send(400, "text/plain", "Bad Request");
            return;
        }

        espressoMachine.command(command, seconds);
    }
    else
    {
        espressoMachine.command(command);
    }

    httpServer.send(204, "text/plain", "No Content");
}

void RestEspressoMachine::handleNotFound()
{
    httpServer.send(404, "text/plain", "Not Found");
}

void RestEspressoMachine::setup()
{
    httpServer.on("/", HTTP_GET, std::bind(&RestEspressoMachine::handleGetRoot, this));
    httpServer.on("/state", HTTP_GET, std::bind(&RestEspressoMachine::handleGetState, this));
    httpServer.on("/command", HTTP_POST, std::bind(&RestEspressoMachine::handlePostCommand, this));
    httpServer.onNotFound(std::bind(&RestEspressoMachine::handleNotFound, this));

    httpServer.enableCORS(true);

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
