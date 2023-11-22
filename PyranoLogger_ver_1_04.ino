// tested with: Arduino IDE 2.1.1 and SAMD Boards Package 1.8.13 - https://github.com/arduino/ArduinoCore-samd/releases
// if MKR ZERO board is bricked press reset button twice fast
// MKR ZERO pinout: https://docs.arduino.cc/static/818db48c8edbf42c21934de80002f970/ABX00012-pinout.png

// required for use of lcdgfx oled driver on MKR ZERO: version 1.1.1 – newer versions don't compile with MKR ZERO
// alternatively: comment out in lcdgfx/src/lcd-hal/UserSettings.h
// //#define CONFIG_ESP32_I2C_ENABLE
// //#define CONFIG_ESP32_SPI_ENABLE

/******************************************************************************
                               --- PyranoLogger ---
 
 A DIY Arduino insolation logger.
 
 This device helps to measure the power of the sun – irradiance (W/m2) from the
 hemisphere – at a given location. It helps you to understand how much solar PV
 energy can be harvested and what is the best orientation of the panels.

 The PyranoLogger is an extened version of the PyronaMeter. It is derived from
 the DIY Irradiation Meter with Arduino that can be found here:
 https://solarduino.com/diy-irradiation-meter-with-arduino/

 The base principle is that we can use a PV panel to read the irrradiance level,
 if we take into account that the value of the short-circuit current (Isc) is
 directly proportional to the intensity of sunlight. We get the Isc from the
 PV panel's datasheet, which will state its value inder standart test conditions
 (STC) – that is, tested at 1000 W/m2 irradiance intensity.

 To get to the actual irrandiance level, we need to measure the actual current
 as a factor of Isc, and multiply that factor with the 1000 W/m2 value.

 In the PyranoLogger/Meter, we measure the actual current (Ix) with an analogue
 to digital converter (ADC) by measuring the voltage over a low value short-cut
 resister (15 Ohm). Using Ohm's Law: V = I * R, thus I = V / R. As we know R,
 and V will be measured by the ADC, I is resulting from this.

 The PyranoLOgger uses the SD-card functionality on the MKRZero board, and uses
 a Real Time Clock (RTC) module to keep track of time even when the devide is
 disconnected from power.

******************************************************************************/

#define firmVer 1.04 // firmware version (Nov. 2023)

#include "const.h"

#include <Wire.h>

// RTC part
#include <TimeLib.h> // https://github.com/PaulStoffregen/Time, uses time_t format
#include <DS3232RTC.h> // https://github.com/JChristensen/DS3232RTC to be used with Time library using the time_t format
 DS3232RTC myRTC;

// SD part
#include <SD.h>
#define FS SD

unsigned long loopStartMillis;
byte hourNow; // current hour
byte minuteNow; // current minute
byte hourLogged; // to detect change of hour

byte dayNow; // current day
byte monthNow; // current month
byte yearNow; // current year

boolean buttonPressed = false;
boolean matrixOn = true; // default start is with matrix display on, turn off to conserve energy

long displayReport = 0; // delay in updating display

// InSol values (PV panel insolation)
int Vx; // raw ADC measurement of Ishortcut from panel in mV
int Ix; // raw current
int insolPower; // available measured PV power calculation
int insolPowerAvg; // averaged insolPower over 60 seconds

// typical 1 panel output
int onePanel; // output 1 panel 1,74m2 / 20% eff (350Wp)

bool sdCardAvailable = false;

#ifdef __SD_H__
void sdTimeCallback(uint16_t* date, uint16_t* time) {
  *date = FAT_DATE(year(), month(), day());
  *time = FAT_TIME(hour(), minute(), second());
}
#endif

boolean statsSaved = false;

/******************************************************************************
*                                 --- SETUP ---                               *
******************************************************************************/

void setup() {
  
    Wire.begin();
    Wire.setClock(400000); // 400000

// buttonSetup ();
    insolSetup();
    buttonSetup();
    displaySetup();
    matrixSetup();

// setup serial communication
    Serial.begin(115200);  // TX can be used if Serial is not used

// start clock module
    rtcSetup();

  #ifdef __SD_H__
    pinMode(NET_SS_PIN, OUTPUT);
    digitalWrite(NET_SS_PIN, HIGH); // unselect network device on SPI bus
    if (!SD.begin(SD_SS_PIN)) {
    sdCardAvailable = false;  // -------------------> output to display
    } else {
    SdFile::dateTimeCallback(sdTimeCallback);
    sdCardAvailable = true;
  } 
  #endif

// set DAC resulution to full 1024
  analogWriteResolution(10); // default is 8 bits (0-256)

  insolStatsSetup();
  watchdogSetup();

}

/******************************************************************************
*                                 --- LOOP ---                                *
******************************************************************************/

void loop() {
  
    loopStartMillis = millis();
    hourNow = hour();
    minuteNow = minute();

    yearNow = year();
    monthNow = month();
    dayNow = day();

    insolStatsLoop();

    watchdogLoop();
    
//  user interface
    // buttonPressed = false; this would create a constant "false"
    buttonLoop();
    if (buttonPressed == true) { 
    // matrixToggle();
    }
    
// display control    
    if (loopStartMillis - displayReport > 1000) { // display update timer 1 sec, to avoid flickering display values
    displayReport = loopStartMillis;
    displayLoop();
    matrixLoop();
    }

    insolLoop();
    RTCloop();

}


void shutdown() {
  insolStatsSave();
  watchdogLoop();
}