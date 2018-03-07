
#ifndef CLIENT_CONTROLLER_
#define CLIENT_CONTROLLER_
#include <Arduino.h>
#include <Basecamp.hpp>
#include "client/client_base.cpp"
#include "client/client_boardlight.cpp"
#include "config.cpp"

class ClientController
{

  private:
    ClientBase *Clients[1];
    byte ClientIndex;
    Basecamp *_basecamp;
    //Basecamp iot_ctrl;

  public:
    ClientController(Basecamp *basecamp)
    {
        _basecamp = basecamp;
        Clients[0] = new ClientBoardlight("home/livingroom/light/boardlights", _basecamp);
        ClientIndex = 1;
    }

    void toogleOnOff(bool on)
    {
        Clients[ClientIndex - 1]->toogleOnOff(on);
    }

    byte set(char setting[], CtrlModeReturn value)
    {
        Serial.print("ClientController:Set Call for setting:");
        Serial.println(setting);
        Clients[ClientIndex - 1]->set(setting, value);
    }

    void switchToNextClient()
    {
        Serial.print("Current Client is");
        Serial.println(ClientIndex);
        if (ClientIndex == G_CLIENT_COUNT)
        {
            ClientIndex = 1;
        }
        else
        {
            ClientIndex++;
        }
        Serial.print("Switch to Client");
        Serial.println(ClientIndex);
    }
    void switchToClient(int index)
    {
        ClientIndex = index;
    }
};

#endif