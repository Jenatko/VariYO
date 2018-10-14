#include "button_control.h"
#include "arduino.h"




int BUTTONCONTROL::getFlag() {
  return flag;

}

void BUTTONCONTROL::buttonUpdate() {


  if (!digitalRead(BUTTON_UP)) {
    buttonPressed = UP;
    if (wasPressed == 0)
      flag = 1;
    wasPressed = 1;

  }
  else if (!digitalRead(BUTTON_DOWN)) {
    buttonPressed = DOWN;
    if (wasPressed == 0)
      flag = 1;
    wasPressed = 1;

  }
  else if (!digitalRead(BUTTON_LEFT)) {
    buttonPressed = LEFT;
    if (wasPressed == 0)
      flag = 1;
    wasPressed = 1;

  }
  else if (!digitalRead(BUTTON_RIGHT)) {
    buttonPressed = RIGHT;
    if (wasPressed == 0)
      flag = 1;
    wasPressed = 1;

  }
  else if (!digitalRead(BUTTON_CENTER)) {
    buttonPressed = PRESS;
    if (wasPressed == 0)
      flag = 1;
    wasPressed = 1;

  }
  else
    wasPressed = 0;


}

int BUTTONCONTROL::getButtonPressed() {
  flag = 0;
  int temp = buttonPressed;
  buttonPressed = 0;
  return temp;

}

BUTTONCONTROL buttons;



