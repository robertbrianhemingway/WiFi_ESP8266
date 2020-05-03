/**
 *  200503 starting to use GitHub as a repository
 * 
 *  200425 trying a Duemilanove as controller
 *  for garage and backyard LEDs
 *
 *  200502 changing board to WiFi ESP8266
 *  in order to get email notifications of triggers
 *  {Notes: No interrupts and do not use D2}
 *
 *  Remember pin ids for arduino are different to those
 *  on the actual board.  See 200501 Notes on WiFi ESP8266.md
 *  In this sketch both
 *  #define sirenPin 13 and #define sirenPin D7
 *  work
 *
 * There is only one analog pin, which I will use for ldrPin
 * and set a time on in the software
 * 
 * DO NOT USE GPIO 2 (D2) - stops uploads and normal functioning
 */
#define     times 3    // # of flashes of led 13
// to indicate successful upload
// Arduino ID of GPIO 13 is board pin D7

#include <Arduino.h>
#include <BlynkSimpleEsp8266.h>
#include "LEDPIR_Unit.h"
#include <WidgetRTC.h>

#define  sirenPin   D7
#define  ldrPin     A0
#define  nighttimeLimit 40  // analogRead(A0), Low = night
//#define  timingPot  A1  // only 1 analog pin on WiFi ESP8266

// -------------------------- Variables ---
String version = "200502 v0a";
LEDUnit     Front(D1,D2);     // led pin, pir pin
LEDUnit     Back(D3,D5);

WidgetLED LEDs(V0);
WidgetTerminal terminal(V2);
WidgetRTC rtc;
byte      sirenValue;  // used to write to Blynk LED widget V0
byte      lastSirenValue; // so we don't write unnecessaarily to Blynk

char auth[] = "RFQizkZailhQNgGQ2t_1QMKPIFqSuq3j";
const char* ssid = "OPTUS_369ECE";
const char* pass = "borelleres79249";
bool  eMailButtonState;
// -------------------------- functions ---
BLYNK_WRITE(V1) {
  eMailButtonState = param.asInt();
}
BLYNK_CONNECTED() {
  // Synchronize time on connection
  rtc.begin();
  delay(2000);
}
String getDateTime() {  // returns a String such as '19/03/2020<14:18>'
  String curTime = String(hour())+":"+minute()+":"+second();
  String curDate = String(day())+"/"+month()+"/"+year();
  return curDate+"<"+curTime+"> ";  
}
void terminalWrite(String message) { // writes date time message to terminal
    terminal.print(getDateTime());
    terminal.println(message);
    terminal.flush();
}
void setSirenOn() {
    // irrispective of day or night
    digitalWrite(sirenPin, HIGH);
    sirenValue = 255;
}
void setSirenOff() {
    digitalWrite(sirenPin, LOW);
    sirenValue = 50;
}

byte    isItNighttime() {
    /*
     *  returns true if it is isItNighttime
     *  as determined by analog value on A0
     */
     return (analogRead(ldrPin) < nighttimeLimit);
     // typically <32 when dark
}
/* // not available on WiFi ESP8266 which has 1 analog pin only
int readTimingPot() {
    // simply return the pot value
    return analogRead(timingPot);
}
unsigned long getTime2StayOn() {
    /*
     * must map A1 pot values to 1 to 20s
     * 0 -> 1000
     * 700 -> 20000
     */
/*    return map(readTimingPot(),0,700,1000,20000);
}
*/
// -------------------------- ISRs --------
void Front_ISR() {  // keep simple and short
    if (isItNighttime()) {
        setSirenOn();
        Front.setOn();  // sets ledPin high, records time on
        Back.setOn();
    }
}

void Back_ISR() {  // keep simple and short
    if (isItNighttime()) {
        setSirenOn();
        Back.setOn();  // sets ledPin high, records time on
        Front.setOn();
    }
}

// --------------------------
void flash13(byte _times) { // to signal a sucessful upload
    if (_times < 0) _times = 3; // just in case
    for (int i=0;i<_times;i++) {
        setSirenOn();
        delay(1000);
        digitalWrite(sirenPin, LOW);
        delay(1000);
    }
}
