#ifndef MODE_COLOR_
#define MODE_COLOR_
#include "led_controller/led.cpp"
#include "CTRLModes/mode_base.cpp"
class CtrlModeColor : public CtrlModeBase
{
  public:
    CtrlModeColor(LED_CONTROLLER *led) : CtrlModeBase(led){};
    CtrlModeReturn processVal()
    {
        Serial.print("CtrlModeColor. Input Encoder Value->");
        Serial.println(CtrlModeBase::val);
        uint32_t mappedVal = map(CtrlModeBase::val, 0, G_ROTATION_ENC_LOOP_COUNT, 0, 360); //hue is 0-360
        _led_service->setHue(mappedVal);
        CtrlModeReturn ret;
        Serial.print("CtrlModeBrightness. mapped Color return Value->");
        Serial.println(mappedVal);
        ret.val1 = mappedVal;
        return ret;
    }
    /*
    void setHue(byte hue)
    {
        _led_service->setHue(hue);
    }
    */
};
#endif