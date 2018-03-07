#ifndef CONFIG_
#define CONFIG_
//Globals

const int PIN_ROTENC_A = 15;
const int PIN_ROTENC_B = 4;
const int PIN_ROTENC_PRESS = 13;
const int PIN_ROTENC_VCC = -1;
const int PIN_LED_RED = 19;
const int PIN_LED_GREEN = 5;  
const int PIN_LED_BLUE = 18; 
const int PIN_LED_WHITE = 21;
const int PIN_MODE_1_LED = 25;
const int PIN_MODE_2_LED = 26;
const int PIN_MODE_3_LED = 27;
const int G_ROTATION_ENC_LOOP_COUNT = 360;
//const int G_BRIGHTNESS_RES = 100; //How many encoderclicks to go from 0 brightness to full
const int G_ROTATION_TIMEOUT = 1000; //ms
const int G_CTRLMODES_COUNT = 2; //How many modes (control brigtness + control color = 2)
const int G_CLIENT_COUNT = 1; //At the moment there is only the lightboards one client. later with volume controll this will become 2
//const int g_colorToPwmValRatio = math.floor(g_pwm_max / g_color_max);
const int G_MAX_HSL_RGB_BRIGHTNESS = 50; //HSL color 50+ look weird on most rgb leds.
//const int G_WHITE_LED_LUMI_THRESHOLD = 50; //The value where the white led strip hooks up to glow with the rgb LED strip 
                                            //Goal is to intense brightnes for very bright colors to illuminate the whole room
const int G_TOUCH_THRESHOLD = 40;    
const unsigned long G_TOUCH_RELEASE_TIME_MS = 500.0;                                        
#endif                                            