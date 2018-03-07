#ifndef LED_CONTROLLER_
#define LED_CONTROLLER_

#include <Arduino.h>

#include <Arduino.h>
#include <led_controller/led_rgbw_state.cpp>
#include <led_controller/led_ctrl_mode_state.cpp>

class LED_CONTROLLER
{
  private:
    int PIN_LED_RED;
    int PIN_LED_GREEN;
    int PIN_LED_BLUE;
    int PIN_LED_WHITE;

    int PIN_LED_MODE1;
    int PIN_LED_MODE2;
    int PIN_LED_MODE3;

    const int LED_CHANNEL_RED = 1;
    const int LED_CHANNEL_GREEN = 2;
    const int LED_CHANNEL_BLUE = 3;
    const int LED_CHANNEL_WHITE = 4;

    const int LED_CHANNEL_MODE1 = 5;
    const int LED_CHANNEL_MODE2 = 6;
    const int LED_CHANNEL_MODE3 = 7;

    LED_RGBW_STATE current_led_rgbw_state;
    LED_CTRL_MODE_STATE current_led_ctrl_mode_state = LED_CTRL_MODE_STATE(1);

    /*
    int LAST_RED_STATE = 0;
    int LAST_GREEN_STATE = 0;
    int LAST_BLUE_STATE = 0;
    int LAST_WHITE_STATE = 0;
    int CURRENT_HUE = 360;
    bool powerStatus = false;
    */
  public:
    LED_CONTROLLER(int pin_led_red, int pin_led_green, int pin_led_blue, int pin_led_white, int pin_mode_1_led, int pin_mode_2_led, int pin_mode_3_led)
    {

        PIN_LED_RED = pin_led_red;
        PIN_LED_GREEN = pin_led_green;
        PIN_LED_BLUE = pin_led_blue;
        PIN_LED_WHITE = pin_led_white;

        PIN_LED_MODE1 = pin_mode_1_led;
        PIN_LED_MODE2 = pin_mode_2_led;
        PIN_LED_MODE3 = pin_mode_3_led;
        //ESP32
        //RGBW LEDs
        ledcAttachPin(PIN_LED_RED, LED_CHANNEL_RED); // assign RGB led pins to channels
        ledcAttachPin(PIN_LED_GREEN, LED_CHANNEL_GREEN);
        ledcAttachPin(PIN_LED_BLUE, LED_CHANNEL_BLUE);
        ledcAttachPin(PIN_LED_WHITE, LED_CHANNEL_WHITE);
        //MODE LEDs
        ledcAttachPin(PIN_LED_MODE1, LED_CHANNEL_MODE1);
        ledcAttachPin(PIN_LED_MODE2, LED_CHANNEL_MODE2);
        ledcAttachPin(PIN_LED_MODE3, LED_CHANNEL_MODE3);

        //RGBW LEDs
        ledcSetup(LED_CHANNEL_RED, 12000, 8); // 12 kHz PWM, 8-bit resolution
        ledcSetup(LED_CHANNEL_GREEN, 12000, 8);
        ledcSetup(LED_CHANNEL_BLUE, 12000, 8);
        ledcSetup(LED_CHANNEL_WHITE, 12000, 8);
        //MODE LEDs
        ledcSetup(LED_CHANNEL_MODE1, 12000, 8);
        ledcSetup(LED_CHANNEL_MODE2, 12000, 8);
        ledcSetup(LED_CHANNEL_MODE3, 12000, 8);

        ledcWrite(LED_CHANNEL_RED, 0);
        ledcWrite(LED_CHANNEL_GREEN, 0);
        ledcWrite(LED_CHANNEL_BLUE, 0);
        ledcWrite(LED_CHANNEL_WHITE, 0);

        ledcWrite(LED_CHANNEL_MODE1, 0);
        ledcWrite(LED_CHANNEL_MODE2, 0);
        ledcWrite(LED_CHANNEL_MODE3, 0);
    }

    bool getPowerStatus()
    {
        return current_led_rgbw_state.powerStatus;
    }

    void setPowerStatus(bool on)
    {
        setPowerStatusRGBW(on);
        //setPowerStatusModeLeds(on);
    }
    void setPowerStatusRGBW(bool on)
    {
        if (on && !current_led_rgbw_state.powerStatus)
        {
            writeAnalogRGBW(current_led_rgbw_state.getPWMVals());
            current_led_rgbw_state.powerStatus = true;
        }
        else if (!on)
        {
            writeAnalogRGBW({0, 0, 0, 0});
            current_led_rgbw_state.powerStatus = false;
        }
    }
    void setPowerStatusModeLeds(bool on)
    {
        if (on && !current_led_ctrl_mode_state.powerStatus)
        {
            current_led_ctrl_mode_state.powerStatus = true;
            writeAnalogModeLeds(current_led_ctrl_mode_state.getPWMVals());
        }
        else if (!on)
        {
            current_led_ctrl_mode_state.powerStatus = false;
            writeAnalogModeLeds({0, 0, 0});
        }
    }

    void setCtrlMode(byte modeNo){
        current_led_ctrl_mode_state.setCtrlMode(modeNo);
        writeAnalogModeLeds(current_led_ctrl_mode_state.getPWMVals());
    }

    byte getCtrlMode(){
        return current_led_ctrl_mode_state.getCtrlMode();
    }

    void setColor(RGB_COLOR color)
    {
        current_led_rgbw_state.setColor(color);
        writeAnalogRGBW(current_led_rgbw_state.getPWMVals());
    }

    void setHue(int hue)
    {
        current_led_rgbw_state.setHue(hue);
        writeAnalogRGBW(current_led_rgbw_state.getPWMVals());
    }

    RGB_COLOR getColor()
    {
        return current_led_rgbw_state.getColorRGB();
    }

    //@brightness int 1-100 (0%-100%)
    void setRGBBrightness(byte brightness)
    {
        current_led_rgbw_state.setRGBBrightness(brightness);
        writeAnalogRGBW(current_led_rgbw_state.getPWMVals());
    }

    byte getRGBBrightness()
    {
        return current_led_rgbw_state.getRGBBrightness();
    }

    void setWhiteBrightness(byte val)
    {
        current_led_rgbw_state.setWhiteBrightness(val);
        writeAnalogRGBW(current_led_rgbw_state.getPWMVals());
    }
    int getWhiteBrightness()
    {
        return current_led_rgbw_state.getWhiteBrightness();
    }

    void writeAnalogRGBW(PWM_RGBW pwm)
    {
        //Esp8266 variant
        //Homie.getLogger() << "led: Set RED PIN  " << PIN_LED_RED << " to " << pwm.r << endl;
        //analogWrite(PIN_LED_RED, pwm.r);
        //Homie.getLogger() << "led: Set GREEN PIN  " << PIN_LED_GREEN << " to " << pwm.g << endl;
        //analogWrite(PIN_LED_GREEN, pwm.g);
        //Homie.getLogger() << "led: Set BLUE PIN  " << PIN_LED_BLUE << " to " << pwm.b << endl;
        //analogWrite(PIN_LED_BLUE, pwm.b);
        //Homie.getLogger() << "led: Set WHITE PIN  " << PIN_LED_WHITE << " to " << pwm.w << endl;
        //analogWrite(PIN_LED_WHITE, pwm.w);

        //ESP32 Variant
        ledcWrite(LED_CHANNEL_RED, pwm.r);
        ledcWrite(LED_CHANNEL_GREEN, pwm.g);
        ledcWrite(LED_CHANNEL_BLUE, pwm.b);
        ledcWrite(LED_CHANNEL_WHITE, pwm.w);
    }
    PWM_RGBW readAnalogRGBW()
    {
        return {ledcRead((uint8_t)LED_CHANNEL_RED),
                ledcRead((uint8_t)LED_CHANNEL_GREEN),
                ledcRead((uint8_t)LED_CHANNEL_BLUE),
                ledcRead((uint8_t)LED_CHANNEL_WHITE)};
    }
    PWM_CTRL_MODE_LEDS readAnalogModeLeds()
    {
        return {ledcRead((uint8_t)LED_CHANNEL_MODE1),
                ledcRead((uint8_t)LED_CHANNEL_MODE2),
                ledcRead((uint8_t)LED_CHANNEL_MODE3)};
    }
    void writeAnalogModeLeds(PWM_CTRL_MODE_LEDS pwm)
    {
        ledcWrite(LED_CHANNEL_MODE1, pwm.led_mode1);
        Serial.print("Write mode 1 led to ");
        Serial.println(pwm.led_mode1);
        ledcWrite(LED_CHANNEL_MODE2, pwm.led_mode2);
        Serial.print("Write mode 2 led to ");
        Serial.println(pwm.led_mode2);
        ledcWrite(LED_CHANNEL_MODE3, pwm.led_mode3);
        Serial.print("Write mode 3 led to ");
        Serial.println(pwm.led_mode3);
    }
};
#endif