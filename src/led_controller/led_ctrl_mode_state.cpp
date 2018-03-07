#ifndef LED_CTRL_MODE_STATE_
#define LED_CTRL_MODE_STATE_
#include <Arduino.h>
//#include <Homie.h>

struct PWM_CTRL_MODE_LEDS
{
    int led_mode1;
    int led_mode2;
    int led_mode3;
};

//The remote has 3 controll modes (color, brightness, volume)
class LED_CTRL_MODE_STATE
{
  private:
    const int pwm_resolution = 255;
    const byte brightness = 50;
    byte mode;

  public:
    bool powerStatus = true;
    LED_CTRL_MODE_STATE(byte ctrl_mode_no)
    {
        mode = ctrl_mode_no;
    }

    void setCtrlMode(byte ctrl_mode_no)
    {
        mode = ctrl_mode_no;
    }

    byte getCtrlMode()
    {
        return mode;
    }

    PWM_CTRL_MODE_LEDS getPWMVals()
    {
        switch (mode)
        {
        case 1:
            return {brightness, 0, 0};
            break;
        case 2:
            return {0, brightness, 0};
            break;
        case 3:
            return {0, 0, brightness};
            break;
        }
    }

};
#endif