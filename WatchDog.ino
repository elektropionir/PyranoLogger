// ** Checks for stalled or locked-up hardware & software lockups – and triggers a reset **

#include <WDTZero.h> // https://github.com/javos65/WDTZero

WDTZero wdt;

void watchdogSetup() {
  wdt.attachShutdown(watchdogShutdown);
  wdt.setup(WDT_SOFTCYCLE32S);
}

void watchdogLoop() {
  wdt.clear();
}

void watchdogShutdown() {
  shutdown();
}