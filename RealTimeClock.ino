// ** Real Time Clock DS3231 **

// This code is used: https://github.com/PaulStoffregen/Time/blob/master/ with https://github.com/JChristensen/DS3232RTC

static bool gotRTC = false;

void rtcSetup() {

 myRTC.begin();
 
//  setTime(12, 8, 00, 15, 10, 2023); // set the system time to hh,,mm,,ss, dd, mm, yyyy
//  myRTC.set(now()); // set the RTC from the system time

  setSyncProvider(myRTC.get); // time source is set to RTC
  time_t t = myRTC.get();

if (timeStatus() != timeSet) { // could not get time
gotRTC = false;
}
else {
  gotRTC = true;
  }
}

void RTCloop()
{  
// time_t t = myRTC.get();
}