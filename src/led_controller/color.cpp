#ifndef COLOR_
#define COLOR_

#include <stdio.h>
#include <string>
#include <math.h>

using std::max;
using std::min;
using std::string;

class RGB_COLOR
{
  public:
    unsigned char R;
    unsigned char G;
    unsigned char B;

    RGB_COLOR(unsigned char r, unsigned char g, unsigned char b)
    {
        R = r;
        G = g;
        B = b;
    }
    RGB_COLOR() //default just white
    {
        R = 255;
        G = 255;
        B = 255;
    }
    bool Equals(RGB_COLOR rgb)
    {
        return (R == rgb.R) && (G == rgb.G) && (B == rgb.B);
    }
};

class HSL_COLOR
{
  public:
    double H;
    double S;
    double L;

    HSL_COLOR(double h, double s, double l)
    {
        H = h;
        S = s;
        L = l;
    }
    HSL_COLOR() //default just white
    {
        H = 1;
        S = 1;
        L = 1;
    }
    bool Equals(HSL_COLOR hsl)
    {
        return (H == hsl.H) && (S == hsl.S) && (L == hsl.L);
    }
};

class CONVERT_COLOR
{
  public:
    CONVERT_COLOR();
    static RGB_COLOR HEX2RGB(string hex)
    {
        if (hex[0] == '#')
            hex = hex.erase(0, 1);

        unsigned char r = (unsigned char)HexadecimalToDecimal(hex.substr(0, 2));
        unsigned char g = (unsigned char)HexadecimalToDecimal(hex.substr(2, 2));
        unsigned char b = (unsigned char)HexadecimalToDecimal(hex.substr(4, 2));
        return RGB_COLOR(r, g, b);
    }


    /**
     * Converts an HSL color value to RGB. Conversion formula
     * adapted from http://en.wikipedia.org/wiki/HSL_color_space.
     * Assumes h, s, and l are contained in the set [0, 1] and
     * returns r, g, and b in the set [0, 255].
     *
     * @param   Number  h       The hue
     * @param   Number  s       The saturation
     * @param   Number  l       The lightness
     * @return  Array           The RGB representation
     */
    static RGB_COLOR HSL2RGB(HSL_COLOR hslc)
    {
        double h = hslc.H;
        double s = hslc.S;
        double l = hslc.L;

        double r, g, b;

        if (s == 0)
        {
            r = g = b = l; // achromatic
        }
        else
        {
            double q = l < 0.5 ? l * (1 + s) : l + s - l * s;
            double p = 2 * l - q;
            r = hue2rgb(p, q, h + 1 / 3.0);
            g = hue2rgb(p, q, h);
            b = hue2rgb(p, q, h - 1 / 3.0);
        }
        return RGB_COLOR(r*255,g*255,b*255);
    };

    static HSL_COLOR RGB2HSL(RGB_COLOR rgb)
    {
        double rd = (double)rgb.R / 255;
        double gd = (double)rgb.G / 255;
        double bd = (double)rgb.B / 255;
        double max = threeway_max(rd, gd, bd);
        double min = threeway_min(rd, gd, bd);
        double h, s, l = (max + min) / 2;

        if (max == min)
        {
            h = s = 0; // achromatic
        }
        else
        {
            double d = max - min;
            s = l > 0.5 ? d / (2 - max - min) : d / (max + min);
            if (max == rd)
            {
                h = (gd - bd) / d + (gd < bd ? 6 : 0);
            }
            else if (max == gd)
            {
                h = (bd - rd) / d + 2;
            }
            else if (max == bd)
            {
                h = (rd - gd) / d + 4;
            }
            h /= 6;
        }
        return HSL_COLOR(h, s, l);
    }

  private:
    static double threeway_max(double a, double b, double c)
    {
        return max(a, max(b, c));
    }

    static double threeway_min(double a, double b, double c)
    {
        return min(a, min(b, c));
    }

    static double hue2rgb(double p, double q, double t)
    {
        if (t < 0)
            t += 1;
        if (t > 1)
            t -= 1;
        if (t < 1 / 6.0)
            return p + (q - p) * 6 * t;
        if (t < 1 / 2.0)
            return q;
        if (t < 2 / 3.0)
            return p + (q - p) * (2 / 3.0 - t) * 6;
        return p;
    }

    static int HexadecimalToDecimal(string hex)
    {
        int hexLength = hex.length();
        double dec = 0;

        for (int i = 0; i < hexLength; ++i)
        {
            char b = hex[i];

            if (b >= 48 && b <= 57)
                b -= 48;
            else if (b >= 65 && b <= 70)
                b -= 55;

            dec += b * pow(16, ((hexLength - i) - 1));
        }

        return (int)dec;
    }
};
#endif