#include "CTRLModes/mode_base.cpp"
#include "CTRLModes/mode_color.cpp"
#include "CTRLModes/mode_brightness.cpp"
#include "led_controller/led.cpp"
#include "config.cpp"

class CtrlModeController
{

  private:
    CtrlModeBase *CtrlModes[G_CTRLMODES_COUNT];
    byte CtrlModeIndex;
    LED_CONTROLLER led = LED_CONTROLLER(PIN_LED_RED, PIN_LED_GREEN, PIN_LED_BLUE, PIN_LED_WHITE, PIN_MODE_1_LED, PIN_MODE_2_LED, PIN_MODE_3_LED);
    //Basecamp iot_ctrl;

  public:
    CtrlModeController()
    {
        CtrlModeIndex = 0;
        led.setCtrlMode(CtrlModeIndex);
        //led = LED_CONTROLLER(PIN_LED_RED, PIN_LED_GREEN, PIN_LED_BLUE, PIN_LED_WHITE, PIN_MODE_1_LED, PIN_MODE_2_LED, PIN_MODE_3_LED);
        LED_CONTROLLER *p_led = &led;

        CtrlModes[0] = new CtrlModeColor(p_led);
        CtrlModes[1] = new CtrlModeBrightness(p_led);
        //CtrlModes[1] = new CtrlModeBrightness(p_led);
        //CtrlModes[2] = new CtrlModeVolume(p_led);
        //iot_ctrl.begin();
    }
    byte getCtrlModeIndex()
    {
        return CtrlModeIndex;
    }
    short addValue(short inputVal)
    {
        return CtrlModes[CtrlModeIndex - 1]->addValue(inputVal);
    }
    void setValue(short input)
    {
        CtrlModes[CtrlModeIndex - 1]->setValue(input);
    }
    short getValue()
    {
        CtrlModes[CtrlModeIndex - 1]->getValue();
    }
    CtrlModeReturn getResult()
    {
        return CtrlModes[CtrlModeIndex - 1]->getResult();
    }

    bool toogleOnOff()
    {
        return CtrlModes[CtrlModeIndex - 1]->toogleOnOff();
    }
    bool getOnOffStatus()
    {
        return CtrlModes[CtrlModeIndex - 1]->getOnOffStatus();
    }

    void setOnOffStatus(bool input)
    {
        CtrlModes[CtrlModeIndex - 1]->setOnOffStatus(input);
    }

    short switchToNextCtrlMode()
    {
        Serial.print("Current mode is");
        Serial.println(CtrlModeIndex);
        if (CtrlModeIndex == G_CTRLMODES_COUNT)
        {
            CtrlModeIndex = 1;
        }
        else
        {
            CtrlModeIndex++;
        }
        Serial.print("Switch to");
        Serial.println(CtrlModeIndex);
        led.setCtrlMode(CtrlModeIndex);
        return CtrlModeIndex;
    }
    void switchToCtrlMode(int index)
    {
        CtrlModeIndex = index;
        led.setCtrlMode(CtrlModeIndex);
    }
};