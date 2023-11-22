// ** The 4 segment LedMatrix display **

// https://github.com/avishorp/TM1637

#include <Arduino.h>
#include <TM1637Display.h>

// Module connection pins (Digital Pins)
TM1637Display matrix(LEDMATRIX_CLOCK_PIN, LEDMATRIX_DATA_PIN);

// Create an array that turns all segments ON
const uint8_t allON[] = {0xff, 0xff, 0xff, 0xff};

// Create an array that turns all segments OFF
const uint8_t allOFF[] = {0x00, 0x00, 0x00, 0x00};

void matrixSetup() {
// Set the brightness to 5 (0=dimmest 7=brightest)
	matrix.setBrightness(5);

// Set all segments ON
	matrix.setSegments(allON); // matrix.setSegments(allOFF);
}

void matrixLoop() {
if (matrixOn) {
// Set all segments ON
//	matrix.setSegments(allON);
  int displayValue = insolPower; // display inverter AC Power  
  matrix.showNumberDec(displayValue, true); //
}
}

void matrixToggle() {
if (matrixOn) {
  matrix.setSegments(allOFF);
  matrixOn = false;
  }
  else if (!matrixOn) {
  matrix.setSegments(allON);
  matrixOn = true;
}
}
