// ** Switches on/off the led-matrix display **

void buttonSetup() {
pinMode(BUTTON_PIN, INPUT);
}

void buttonLoop() {

  const int LONG_PRESS_INTERVAL = 3000; // 3 sec
  const int RESET_PRESS_INTERVAL = 2 * LONG_PRESS_INTERVAL;
  static unsigned long pressStartMillis = 0;
  static boolean longPressDone = false;

  boolean pressed = (digitalRead(BUTTON_PIN) == HIGH);

  if (!buttonPressed) { 
    buttonPressed = pressed; // set the global 'virtual' button press
  }

    // long press handling
  if (pressStartMillis == 0) { // not counting
    if (pressed) { // start counting
      pressStartMillis = loopStartMillis;
    }
  } else if (!pressed) { // stop counting
     pressStartMillis = 0;
     longPressDone = false;
  } else { // counting
    unsigned long d = loopStartMillis - pressStartMillis;
    if (d > RESET_PRESS_INTERVAL) { // resetting button state
  // do something
    
    } else if (!longPressDone && d > LONG_PRESS_INTERVAL) { // long press detected
      matrixToggle();
      longPressDone = true;
    }
  }
}