
#ifndef MODE_BASE_
#define MODE_BASE_
#include <Arduino.h>
#include "config.cpp"
#include "led_controller/led.cpp"
#include "CTRLModes/mode_controller_return.cpp"
class CtrlModeBase
{
  private:

  protected:
    int val;
    virtual CtrlModeReturn processVal() {}

  public:
    LED_CONTROLLER *_led_service;
    CtrlModeBase(LED_CONTROLLER *led)
    {
        _led_service = led;
        val = 0;
    }
    
    bool toogleOnOff(){
        bool currentStatus = _led_service->getPowerStatus();
        _led_service->setPowerStatus(!currentStatus);
        return !currentStatus;
    }
    bool getOnOffStatus(){
        return _led_service->getPowerStatus();
    }
    void setOnOffStatus(bool currentStatus){
        _led_service->setPowerStatus(currentStatus);
    }

    short addValue(short inputVal)
    {
        Serial.print("CtrlModeBase.addValue(Inputval)->");
        Serial.println((val + inputVal));
        if ((val + inputVal) >= G_ROTATION_ENC_LOOP_COUNT)
        {
            val = (val + inputVal) - G_ROTATION_ENC_LOOP_COUNT;
        }
        else if ((val + inputVal) < 0)
        {
            val = (val + inputVal) + G_ROTATION_ENC_LOOP_COUNT;
        }
        else
        {
            val = val + (inputVal);
        }
        Serial.print("CtrlModeBase.addValue(return val)->");
        Serial.println(val);
        return val;
    }
    void setValue(short input){
        val = input;
    }
    short getValue()
    {
        return val;
    }

    CtrlModeReturn getResult()
    {
        return processVal();
    }
};
#endif
