#include <Arduino.h>

#include <Basecamp.hpp>
#include <AiEsp32RotaryEncoder.h>
#include <Preferences.h>
#include "mode_controller.cpp"
#include "client_controller.cpp"
#include "config.cpp"

Basecamp iot_ctrl;
ClientController clients = ClientController(&iot_ctrl);

AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(
    PIN_ROTENC_A,
    PIN_ROTENC_B,
    PIN_ROTENC_PRESS,
    PIN_ROTENC_VCC);
RTC_DATA_ATTR int bootCount = 0;
int deepSleepTimer = 0;
int idleTimer = 0;

typedef enum {
    SETUP = 1,
    RUNNING
} State;
State state = SETUP;

int16_t lastEncoderVal = 0;
Preferences preferences;
CtrlModeController CmodeController;

//Touch handler vars
bool touchDetectedPin33 = false;

/*
Method to print the reason by which ESP32
has been awaken from sleep
*/
void print_wakeup_reason()
{
    esp_sleep_wakeup_cause_t wakeup_reason;

    wakeup_reason = esp_sleep_get_wakeup_cause();

    switch (wakeup_reason)
    {
    case 1:
        Serial.println("Wakeup caused by external signal using RTC_IO");
        break;
    case 2:
        Serial.println("Wakeup caused by external signal using RTC_CNTL");
        break;
    case 3:
        Serial.println("Wakeup caused by timer");
        break;
    case 4:
        Serial.println("Wakeup caused by touchpad");
        break;
    case 5:
        Serial.println("Wakeup caused by ULP program");
        break;
    default:
        Serial.println("Wakeup was not caused by deep sleep");
        break;
    }
}

void rotary_onButtonClick()
{
    Serial.println("ButtonClick.");
    CmodeController.switchToNextCtrlMode();
    deepSleepTimer = 0;
    //rotaryEncoder.disable();
}
int16_t getEncoderDelta(int16_t lastVal, int16_t currentVal)
{
    Serial.print("Main.getEncoderDelta(lastval)->");
    Serial.println(lastVal);
    Serial.print("Main.getEncoderDelta(currentVal)->");
    Serial.println(currentVal);
    if ((lastVal - currentVal) > G_ROTATION_ENC_LOOP_COUNT / 2)
    {
        return (lastVal - currentVal) - G_ROTATION_ENC_LOOP_COUNT;
    }
    else if ((lastVal - currentVal) < (-1 * G_ROTATION_ENC_LOOP_COUNT / 2))
    {
        return (lastVal - currentVal) + (lastVal + currentVal);
    }
    else
    {
        return (lastVal - currentVal);
    }
    /*
    Serial.print("Main.getEncoderDelta(lastval)->");
    Serial.println(lastVal);
    Serial.print("Main.getEncoderDelta(currentVal)->");
    Serial.println(currentVal);
    if (lastVal >= G_ROTATION_ENC_LOOP_COUNT && currentVal <= 0)
    {
        Serial.println("Return1->1");

        return 1;
    }
    else if (lastVal == G_ROTATION_ENC_LOOP_COUNT && currentVal == 0)
    {
        Serial.println("Return2->-1");
        return -1;
    }
    else
    {
        Serial.print("Return3->");
        Serial.println(lastVal - currentVal);
        return lastVal - currentVal;
    }
    */
}

void rotary_loop()
{
    //first lets handle rotary encoder button click
    if (rotaryEncoder.currentButtonState() == BUT_RELEASED)
    {
        //1234we can process it here or call separate function like:
        rotary_onButtonClick();
    }

    //lets see if anything changed
    int16_t encoderDelta = rotaryEncoder.encoderChanged();
    //rotaryEncoder.encoderChanged()  value jumps like fuck. only usable to determine if + or -
    //lets make our own delta val encoderDelta2

    //optionally we can ignore whenever there is no change
    if (encoderDelta == 0)
        return;

    //for some cases we only want to know if value is
    //increased or decreased (typically for menu items)
    if (encoderDelta > 0)
        Serial.println("+");
    if (encoderDelta < 0)
        Serial.println("-");

    //for other cases we want to know what is current value.
    //Additionally often we only want if something changed
    //example: when using rotary encoder to set termostat temperature, or sound volume etc

    //if value is changed compared to our last read
    if (encoderDelta != 0)
    {

        deepSleepTimer = 0;
        //now we need current value

        //Ecnoder
        int16_t encoderValue = rotaryEncoder.readEncoder();
        int16_t encoderDelta2 = getEncoderDelta(lastEncoderVal, encoderValue);
        lastEncoderVal = encoderValue;

        //Encoder value is reliable so we can calcluate our own delta
        CmodeController.addValue(encoderDelta2);

        CtrlModeReturn result = CmodeController.getResult();

        switch (CmodeController.getCtrlModeIndex())
        {
        case 1:
            clients.set("hue", result);
            break;
        case 2:
            clients.set("brightness", result);
            break;
        }
        //process new value. Here is simple output.
        Serial.print("Value: ");
        Serial.println(encoderValue);
        Serial.println("---------------------------------------------------");
    }
}

bool isTouchReleased(short pin)
{
    if (touchRead(pin) < G_TOUCH_THRESHOLD)
    {
        return false;
    }
    Serial.print("TouchReleased for Pin ");
    Serial.println(pin);
    touchDetectedPin33 = false;
    return true;
}

void touchhandler_loop()
{
    if (touchDetectedPin33)
    {
        if (isTouchReleased(33))
        {
            Serial.println("ToogleOnOff");
            clients.toogleOnOff(CmodeController.toogleOnOff());
        }
    }
}

void touchedPin33()
{
    touchDetectedPin33 = true;
}
void load()
{
    for (int i=1; i<=G_CTRLMODES_COUNT;i++){
        //create a namespace ctrMode1, ctrlMode2,...
        char storageNameBaseVal[12] = "ctrlModeVal";
        char storageNameBasePowerStatus[11] = "ctrlModePS";
        char storageNameIndex[1];
        sprintf(storageNameIndex, "%d", i);
        char storageNameVal[13];
        char storageNamePS[12];
        strcpy (storageNameVal,storageNameBaseVal);
        strcat(storageNameVal,storageNameIndex);
        strcpy (storageNamePS,storageNameBasePowerStatus);
        strcat(storageNamePS,storageNameIndex);
        CmodeController.switchToCtrlMode(i);
        CmodeController.setValue(preferences.getUInt(storageNameVal, 1));
        CmodeController.setOnOffStatus(preferences.getUInt(storageNamePS, 1));
        //get Result processes the value and sets the leds
        CmodeController.getResult();
        CmodeController.getOnOffStatus();
    }
    CmodeController.switchToCtrlMode(preferences.getUInt("ctrlMode", 1));
}
void save()
{
    preferences.putUInt("ctrlMode", CmodeController.getCtrlModeIndex());
    for (int i=1; i<=G_CTRLMODES_COUNT;i++){
        //create a namespace ctrMode1, ctrlMode2,...
        char storageNameBaseVal[12] = "ctrlModeVal";
        char storageNameBasePowerStatus[11] = "ctrlModePS";
        char storageNameIndex[1];
        sprintf(storageNameIndex, "%d", i);
        char storageNameVal[13];
        char storageNamePS[12];
        strcpy (storageNameVal,storageNameBaseVal);
        strcat(storageNameVal,storageNameIndex);
        strcpy (storageNamePS,storageNameBasePowerStatus);
        strcat(storageNamePS,storageNameIndex);
        CmodeController.switchToCtrlMode(i);
        preferences.putUInt(storageNameVal,CmodeController.getValue());
        preferences.putUInt(storageNamePS,CmodeController.getOnOffStatus());
    }
}
void setup()
{

    //Serial.begin(115200);
    delay(100);
    //esp_sleep_enable_ext0_wakeup((gpio_num_t)PIN_ROTENC_A, 0); //1 = High, 0 = Low
    //print_wakeup_reason();
    //esp_deep_sleep
    //
    preferences.begin("boardlight", false);
    delay(100);
    rotaryEncoder.begin();
    rotaryEncoder.setBoundaries(0, G_ROTATION_ENC_LOOP_COUNT, true);
    delay(100);
    iot_ctrl.begin();
    delay(100);
    CmodeController.switchToCtrlMode(1);
    delay(100);
    touchAttachInterrupt(33, touchedPin33, G_TOUCH_THRESHOLD);
    delay(100);
    load();
    delay(100);
    state = RUNNING;
}

void loop()
{
    if (state = RUNNING)
    {
        rotary_loop();
        touchhandler_loop();
        deepSleepTimer++;
        idleTimer++;
        delay(50);

        if (deepSleepTimer > 100)
        {
            //led.setPowerStatus(false);
            //Serial.println(deepSleepTimer);
            //Serial.println("Muss schlafi machen");
            //esp_deep_sleep_start();
        }
    }
}
