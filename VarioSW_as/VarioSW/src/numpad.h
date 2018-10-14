/*
 * numpad.h
 *
 * Created: 6/10/2018 18:05:59
 *  Author: Jena
 */ 


#ifndef NUMPAD_H_
#define NUMPAD_H_

#define NUMPAD_BUTTON_SIZE 36
#define NUMPAD_LEFT_OFFSET 30
#define NUMPAD_BOTTOM_OFFSET 195

int numpad(int initial_number);
void numpadInit(int number);
void printNumpadButton(int index, int pressed);




#endif /* NUMPAD_H_ */