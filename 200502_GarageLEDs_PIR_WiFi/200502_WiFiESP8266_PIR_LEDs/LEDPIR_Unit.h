/**
 * 200424 rewrite of a class used in
 * C:\Users\Robert\Documents\PlatformIO\Projects\200424-134832-esp32doit-devkit-v1
 *  usage
 *    LEDUnit frontLED(22,26);
 *    LEDUnit ;backLED(23,27);
 *
 */
#include <arduino.h>
#define setTimeOn 10000   // 10000 milliseconds

 class LEDUnit {  // ==================== class LEDUnit
    // Class Member Variables
    private:
        byte    ledPin;
        byte    pirPin;
        byte    isTriggered;  // true when PIR triggered
        unsigned long    lastTrigger;  // millis() at time of trigger
        unsigned long    time2StayOn;   // = 10000;

    //------------------------------------

    // Constructor
    public:
        LEDUnit(byte _ledPin, byte _pirPin) {
            ledPin = _ledPin;
            pirPin = _pirPin;
            isTriggered = false;
            time2StayOn = setTimeOn; // ***** read LDR if available

            pinMode(ledPin, OUTPUT);
            digitalWrite(ledPin, LOW);
            pinMode(pirPin, INPUT_PULLUP);
        }
        void setOn() {
            /* called by ISR */
            if (!isTriggered) {
                isTriggered = true;
                lastTrigger = millis();
                digitalWrite(ledPin,HIGH);
            }
        }
        byte    getPin() {return pirPin;}
        byte    getPirPinState() {
                  return (digitalRead(pirPin));
        }
        byte    isPirTimeUp() {
            /* polled in loop to turn led off
                returns state of isTriggered
            */
            if (isTriggered) {
                if ((millis()-lastTrigger)>time2StayOn) {
                    //  yes, time is up
                    digitalWrite(ledPin,LOW);
                    isTriggered = false;
                }
            }
            return isTriggered;  // use to keep 1 buzzer
            // synched between 2 pir's
        }
/*
        void setTime2StayOn(unsigned long _ms) {
            /*
             * usually read from an analog input
             * connected to a pot.
             */
/*            time2StayOn = _ms;
        }
*/
 }; // ------------------------------------ end Class LEDUnit
