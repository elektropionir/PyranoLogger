// ** The 128*128 character oled data screen **

#include "lcdgfx.h" // https://github.com/lexus2k/lcdgfx

// standard X/Y positions (16) : 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 128

DisplaySSD1327_128x128_I2C display(-1); // SSD1327 Oled 128 x 128

String s;
char str[16];

void displaySetup() {
    display.setFixedFont( ssd1306xled_font6x8 );
    display.begin();
    display.clear();

      display.printFixed(24, 16, "ELEKTROPIONIR", STYLE_BOLD);
    lcd_delay(2000);
      display.printFixed(28, 48, "PYRANOLOGGER", STYLE_NORMAL);

      // display firmware version
      display.printFixed(28, 64, "version", STYLE_NORMAL);
      sprintf(str,"%.2f",firmVer);
      display.printFixed(76, 64, str, STYLE_NORMAL);

     lcd_delay(2000);   
      display.clear();
}

void displayClear() {
display.clear();
}

void displayLoop() {
  display.setColor(GRAY_COLOR4(200)); // 255 is max level
  
  display.printFixed(28, 0, "PYRANOLOGGER", STYLE_BOLD);

// display Irradiance (W/m2)
   display.setFixedFont( ssd1306xled_font8x16 );
  display.printFixed(0, 12, "SUN", STYLE_NORMAL);
  sprintf(str,"%04d",insolPower); // insol
  display.printFixed(44, 12, str, STYLE_BOLD);
  display.setFixedFont( ssd1306xled_font6x8 );
  display.printFixed(94, 20, "W/m2", STYLE_NORMAL);

  display.setFixedFont( ssd1306xled_font8x16 );
  display.printFixed(0, 28, "AVG", STYLE_NORMAL);
  sprintf(str,"%04d",insolPowerAvg); // insol
  display.printFixed(44, 28, str, STYLE_BOLD);
    display.setFixedFont( ssd1306xled_font6x8 );
  display.printFixed(94, 36, "W/m2", STYLE_NORMAL);
 
  display.setFixedFont( ssd1306xled_font6x8 );

// display kWh insolation per hour
  sprintf(str,"%.2f",((float) insolStatsInsolationPowerHour()/1000.0)); // kWh per hour
  display.printFixed(44, 50, str, STYLE_NORMAL);  
   display.printFixed(72, 50, "SOL/h", STYLE_NORMAL);
  
  // display kWh insolation per day
  sprintf(str,"%.2f",((float) insolStatsInsolationPowerToday()/1000.0)); // kWh on day
  display.printFixed(44, 58, str, STYLE_NORMAL);  
  if ((float) insolStatsInsolationPowerToday()/1000.0 < 10) {
   display.printFixed(72, 58, "SOL/d", STYLE_NORMAL);
  } else { // we move one segment on the display
  display.printFixed(80, 58, "SOL/d", STYLE_NORMAL);
  }

// display kWh insolation per month
  sprintf(str,"%.2f",((float) insolStatsInsolationPowerMonth()/1000.0)); // kWh on day
  display.printFixed(44, 68, str, STYLE_NORMAL);  
  if ((float) insolStatsInsolationPowerMonth()/1000.0 < 10) {
   display.printFixed(72, 68, "SOL/m", STYLE_NORMAL);
  } else { // we move one segment on the display
  display.printFixed(80, 68, "SOL/m", STYLE_NORMAL);
  }
  
  display.setColor(GRAY_COLOR4(75)); // 255 is max level

// display solar sensor (Vx raw)
  display.printFixed(0, 80, "Vsc", STYLE_NORMAL);
  sprintf(str,"%04d",Vx); // voltage in mV measured by ADC
  display.printFixed(44, 80, str, STYLE_NORMAL);
  display.printFixed(72, 80, "mV", STYLE_NORMAL);

// display solar sensor (Ix raw)
    display.printFixed(0, 88, "Isc", STYLE_NORMAL);
    sprintf(str,"%04d",Ix); // power in mW measured by ADC
    display.printFixed(44, 88, str, STYLE_NORMAL);
    display.printFixed(72, 88, "mA", STYLE_NORMAL);

  display.setColor(GRAY_COLOR4(200)); // 255 is max level

// footer

// warning if no SD is available
if (!sdCardAvailable) {
    display.printFixed(98, 108, "NO SD", STYLE_NORMAL);
    }
if (sdCardAvailable && statsSaved) {
    display.printFixed(98, 108, "SD ++", STYLE_NORMAL);
    }
if (sdCardAvailable && !statsSaved) {
  display.printFixed(98, 108, "     ", STYLE_NORMAL);
} 

// display temp
  sprintf(str,"%.1f", myRTC.temperature()/4.0);
  display.printFixed(0, 120, str, STYLE_NORMAL);
  display.printFixed(24, 120, "'", STYLE_NORMAL);
  display.printFixed(28, 120, "C", STYLE_NORMAL);
  
// display day, mont, year
  sprintf(str,"%02d", dayNow); // day
  display.printFixed(44, 120, str, STYLE_NORMAL);
  display.printFixed(56, 120, ".", STYLE_NORMAL);
  sprintf(str,"%02d", monthNow); // minute
  display.printFixed(60, 120, str, STYLE_NORMAL);
  display.printFixed(72, 120, ".", STYLE_NORMAL);
  sprintf(str,"%02d", (yearNow%100-8)); // year %100
  display.printFixed(76, 120, str, STYLE_NORMAL);

// display current time 
  sprintf(str,"%02d",hourNow); // hour
  display.printFixed(98, 120, str, STYLE_NORMAL);
  display.printFixed(110, 120, ":", STYLE_NORMAL);
  sprintf(str,"%02d",minuteNow); // minute
  display.printFixed(114, 120, str, STYLE_NORMAL);
  if (timeStatus() != timeSet) {
    display.printFixed(128, 120, "!", STYLE_NORMAL);
  }


  }



