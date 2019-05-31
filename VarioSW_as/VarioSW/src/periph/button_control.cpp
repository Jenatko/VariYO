#include "button_control.h"
#include "arduino.h"




int BUTTONCONTROL::getFlag() {

	return flag;

}

void BUTTONCONTROL::buttonUpdate() {


	if (!digitalRead(BUTTON_UP)) {
		
		if (wasPressed == 0 && index < 5){
			buttonPressed[index] = UP;
			flag = 1;
			wasPressed = 1;
			index++;
		}


	}
	else if (!digitalRead(BUTTON_DOWN)) {
		
		if (wasPressed == 0 && index < 5){
			buttonPressed[index] = DOWN;
			flag = 1;
			wasPressed = 1;
			index++;
		}

	}
	else if (!digitalRead(BUTTON_LEFT)) {
		
		if (wasPressed == 0 && index < 5){
			buttonPressed[index] = LEFT;
			flag = 1;
			wasPressed = 1;
			index++;
		}

	}
	else if (!digitalRead(BUTTON_RIGHT)) {
		
		if (wasPressed == 0 && index < 5){
			buttonPressed[index] = RIGHT;
			flag = 1;
			wasPressed = 1;
			index++;
		}

	}
	else if (!digitalRead(BUTTON_CENTER)) {
		
		if (wasPressed == 0 && index < 5){
			buttonPressed[index] = PRESS;
			flag = 1;
			wasPressed = 1;
			index++;
		}

	}
	else
	wasPressed = 0;


}

int BUTTONCONTROL::getButtonPressed() {
	/*
	if(index > 0){
	index--;
	}
	int temp = buttonPressed[index];
	if(index == 0){
	flag = 0;
	}
	return temp;
	*/
	if(index > 0){
		index--;
	}
	int temp = buttonPressed[0];
	if(index == 0){
		flag = 0;
	}
	buttonPressed[0] = buttonPressed[1];
	buttonPressed[1] = buttonPressed[2];
	buttonPressed[2] = buttonPressed[3];
	buttonPressed[3] = buttonPressed[4];
	buttonPressed[4] = buttonPressed[5];
	return temp;
	

}

BUTTONCONTROL buttons;



