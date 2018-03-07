
#ifndef CLIENT_BOARDLIGHT_
#define CLIENT_BOARDLIGHT_
#include <Arduino.h>
#include <Basecamp.hpp>
#include <client/client_base.cpp>
#include <CTRLModes/mode_controller_return.cpp>
class ClientBoardlight : public ClientBase
{
  protected:
    void setHue(CtrlModeReturn data)
    {
        char cstr1[16];
        itoa(data.val1, cstr1, 10);
        ClientBase::sendPublish("/rgb/hue/set", cstr1);
    }

    void setOn(CtrlModeReturn data)
    {
        char cstr1[16];
        itoa(data.val1, cstr1, 10);
        ClientBase::sendPublish("/rgbw/on/set", cstr1);
    }

    void setBrightness(CtrlModeReturn data)
    {
        char cstr1[16];
        char cstr2[16];
        itoa(data.val1, cstr1, 10);
        itoa(data.val2, cstr2, 10);
        ClientBase::sendPublish("/rgb/brightness/set", cstr1);
        delay(100);
        ClientBase::sendPublish("/w/brightness/set", cstr2);
    }

  public:
    ClientBoardlight(const char *mqtt_topic, Basecamp *basecamp) : ClientBase(mqtt_topic, basecamp){};

    void set(char setting[], CtrlModeReturn vals)
    {
        Serial.print("ClientBoardlight:set");
        if (strcmp(setting, "hue") == 0)
        {
            Serial.println("hue");
            setHue(vals);
        }
        else if (strcmp(setting, "brightness") == 0)
        {
            Serial.println("brightness");
            setBrightness(vals);
        }
        else if (strcmp(setting, "on") == 0)
        {
            Serial.println("on");
            setOn(vals);
        }
        else
        {
            Serial.println("none");
        }
    };

    void toogleOnOff(bool on)
    {
        if (on)
        {
            ClientBase::sendPublish("/rgbw/on/set", "true");
        }
        else if (!on)
        {
            ClientBase::sendPublish("/rgbw/on/set", "false");
        }
    };
};

#endif