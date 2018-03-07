#ifndef LED_RGBW_STATE_
#define LED_RGBW_STATE_
#include <led_controller/color.cpp>
#include <Arduino.h>
#include <config.cpp>
//#include <Homie.h>

struct PWM_RGBW
{
    int r;
    int g;
    int b;
    int w;
};

class LED_RGBW_STATE
{
  private:
    HSL_COLOR hsl_color;
    byte white_brightness;
    const int pwm_resolution = 255;

  public:
    bool powerStatus = true;
    LED_RGBW_STATE()
    {
        hsl_color = HSL_COLOR(0.166, 1, 0.2); //dark/yellow
        white_brightness = 50;
    }
    LED_RGBW_STATE(HSL_COLOR hsl)
    {
        hsl_color = hsl;
        white_brightness = 100;
    }
    LED_RGBW_STATE(HSL_COLOR hsl, byte white)
    {
        hsl_color = hsl;
        white_brightness = white;
    }
    LED_RGBW_STATE(RGB_COLOR rgb)
    {
        hsl_color = CONVERT_COLOR::RGB2HSL(rgb);
        white_brightness = 100;
    }
    LED_RGBW_STATE(RGB_COLOR rgb, byte white)
    {
        hsl_color = CONVERT_COLOR::RGB2HSL(rgb);
        white_brightness = white;
    }

    void setColor(HSL_COLOR hsl)
    {
        hsl_color = hsl;
    }
    void setHue(int hue)
    {
        hsl_color.H = (hue / 360.0);
    }
    int getHue()
    {
        return (hsl_color.H * 360);
    }
    void setColor(RGB_COLOR rgb)
    {
        hsl_color = CONVERT_COLOR::RGB2HSL(rgb);
    }
    void setRGBBrightness(byte brightness)
    {
        Serial.print("LED_RGBW_STATE.setRGBBrightness input val->");
        Serial.println(brightness);
        hsl_color.L = brightness / 100.0;
        Serial.print("LED_RGBW_STATE.setRGBBrightness calulated brightness val->");
        Serial.println(hsl_color.L);
    }
    byte getRGBBrightness()
    {
        Serial.print("LED_RGBW_STATE.getRGBBrightness hsl_color.L->");
        Serial.println(hsl_color.L);
        Serial.print("LED_RGBW_STATE.getRGBBrightness calculated return val->");
        Serial.println((int)hsl_color.L * 100.0);
        return (int)hsl_color.L * 100.0;
    }
    void setWhiteBrightness(byte brightness)
    {
        white_brightness = brightness;
    }
    HSL_COLOR getColorHSL()
    {
        return hsl_color;
    }
    RGB_COLOR getColorRGB()
    {
        return CONVERT_COLOR::HSL2RGB(hsl_color);
    }
    byte getWhiteBrightness()
    {
        return white_brightness;
    }

    PWM_RGBW getPWMVals()
    {
        RGB_COLOR rgb = CONVERT_COLOR::HSL2RGB(hsl_color);
        return {getPWMVal(rgb.R, 255),
                getPWMVal(rgb.G, 255),
                getPWMVal(rgb.B, 255),
                getPWMVal(white_brightness, 100)};
    }

  private:
    void setPWMVals(PWM_RGBW pwmvals)
    {
        RGB_COLOR rgb;
        rgb.R = getRGBVals(pwmvals.r);
        rgb.G = getRGBVals(pwmvals.g);
        rgb.B = getRGBVals(pwmvals.b);
        white_brightness = getRGBVals(pwmvals.w);
        hsl_color = CONVERT_COLOR::RGB2HSL(rgb);
    }

    int getPWMVal(int val, int base_max)
    {
        return map(val, 0, base_max, 0, 255);
    }
    int getRGBVals(int val)
    {
        return map(val, 0, pwm_resolution, 0, 255);
    }
};
#endif