
#ifndef CLIENT_
#define CLIENT_
#include <Arduino.h>
#include <Basecamp.hpp>
#include <CTRLModes/mode_controller_return.cpp>

class ClientBase
{
  protected:
    char _topic[50];
    Basecamp *_basecamp;
    void sendPublish(const char *message, const char *payload)
    {
        char _message[100];
        strcpy(_message, _topic);
        strcat(_message, message);
        Serial.print("Send mqtt:'");
        Serial.print(_message);
        Serial.print("' -payload> ");
        Serial.println(payload);
        _basecamp->mqtt.publish(_message, 1, true, payload);
    }

  public:
    ClientBase(const char *mqtt_topic, Basecamp *basecamp)
    {
        strcpy(_topic,mqtt_topic);
        _basecamp = basecamp;
    }

    virtual void toogleOnOff(bool on){}

    virtual void set(char setting[], CtrlModeReturn vals){};
};

#endif