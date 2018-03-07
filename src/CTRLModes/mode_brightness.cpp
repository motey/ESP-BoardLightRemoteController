#ifndef MODE_BRIGHTNESS_
#define MODE_BRIGHTNESS_
#include "led_controller/led.cpp"
#include "CTRLModes/mode_base.cpp"
class CtrlModeBrightness : public CtrlModeBase
{
  public:
    CtrlModeBrightness(LED_CONTROLLER *led) : CtrlModeBase(led){};
    CtrlModeReturn processVal()
    {
        /*
        100 -
            -             /\
            -            /  \
            -           /    \
          0 -          /      \ <-----White Brightness
        100 -         __________
            -        /          \
            -       /            \
            -      /              \
            -     /                \
          0 -    /                  \ <-------Rgb Brigthness
Rotary Encoder->|0   |120      |240  |360
        */
        //RGB Brigthness
        Serial.print("CtrlModeBrightness. Input encoder Value->");
        Serial.println(CtrlModeBase::val);
        uint32_t mappedRGBVal = getRGBBrightnessVal(CtrlModeBase::val); //brighness is 0-100
        //Limit max brigthness
        mappedRGBVal = map(mappedRGBVal, 0, 100.0, 0, G_MAX_HSL_RGB_BRIGHTNESS);
        //White brightness
        uint32_t mappedWVal = getWhiteBrightnessVal(CtrlModeBase::val);
        Serial.print("CtrlModeBrightness. mapped RGB Brightness return Value->");
        Serial.println(mappedRGBVal);
        _led_service->setRGBBrightness(mappedRGBVal);
        Serial.print("CtrlModeBrightness. mapped White Brightness return Value->");
        Serial.println(mappedWVal);
        _led_service->setWhiteBrightness(mappedWVal);
        CtrlModeReturn ret;
        ret.val1 = mappedRGBVal;
        ret.val2 = mappedWVal;
        return ret;
    }

    int getRGBBrightnessVal(int RotaryEncoderValue)
    {
        //if 0 - 120 -> 0%-100%
        if (RotaryEncoderValue <= (G_ROTATION_ENC_LOOP_COUNT / 3))
        {
            return map(RotaryEncoderValue, 0, G_ROTATION_ENC_LOOP_COUNT / 3, 0, 100);
            //return ((brightval - G_ROTATION_ENC_LOOP_COUNT / 3) * (-1)) * 2;
        }
        //if 240-360 -> 100%-0%
        else if (RotaryEncoderValue >= ((G_ROTATION_ENC_LOOP_COUNT / 3) * 2))
        {
            return map(RotaryEncoderValue, ((G_ROTATION_ENC_LOOP_COUNT / 3) * 2), G_ROTATION_ENC_LOOP_COUNT, 100, 0);
        }
        //if 120-240 -> 100%
        else if (RotaryEncoderValue > (G_ROTATION_ENC_LOOP_COUNT / 3) && RotaryEncoderValue < ((G_ROTATION_ENC_LOOP_COUNT / 3) * 2))
        {
            return 100;
        }
    }
    int getWhiteBrightnessVal(int RotaryEncoderValue)
    { //if 0-120 or 240-360 -> 0%
        if (RotaryEncoderValue < (G_ROTATION_ENC_LOOP_COUNT / 3) || RotaryEncoderValue > ((G_ROTATION_ENC_LOOP_COUNT / 3) * 2))
        {
            return 0;
        }
        //if 120-180 -> 0%-100%
        else if (RotaryEncoderValue > (G_ROTATION_ENC_LOOP_COUNT / 3) && RotaryEncoderValue <= (G_ROTATION_ENC_LOOP_COUNT / 2))
        {
            return map(RotaryEncoderValue, G_ROTATION_ENC_LOOP_COUNT / 3, G_ROTATION_ENC_LOOP_COUNT / 2, 0, 100);
        }
        //if 180-240 -> 100%-0%
        else if (RotaryEncoderValue > (G_ROTATION_ENC_LOOP_COUNT / 2) && RotaryEncoderValue <= ((G_ROTATION_ENC_LOOP_COUNT / 3) * 2))
        {
            return map(RotaryEncoderValue, G_ROTATION_ENC_LOOP_COUNT / 2, (G_ROTATION_ENC_LOOP_COUNT / 3) * 2, 100, 0);
        }
    }

    /*
    void setBrightness(byte hue)
    {
        _led_service->setRGBBrightness(hue);
    }
    */
};

#endif