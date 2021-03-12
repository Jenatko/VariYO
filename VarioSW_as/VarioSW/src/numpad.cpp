#include <GxEPD2_BW.h>

//#include <GxGDEP015OC1/GxGDEP015OC1.h>
#include "numpad.h"
#include "button_control.h"

#include <Fonts/FreeMonoBold24pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>

int numpad(int initial_number) {
	int number = 0;
	int selected = 0;
	numpadInit(number);
	printNumpadButton(selected, 1);

	while (1) {
	//	buttons.buttonUpdate();


		if (buttons.getFlag()) {
			while(buttons.getFlag()){
				printNumpadButton(selected, 0);
				switch (buttons.getButtonPressed()) {
					case (0):
					selected -= 4;
					if (selected < 0)
					selected = 16 + selected;
					selected = selected % 16;
					printNumpadButton(selected, 1);
					break;

					case (1):
					selected += 4;
					if (selected < 0)
					selected = 16 + selected;
					selected = selected % 16;
					printNumpadButton(selected, 1);
					break;

					case (2):
					selected -= 1;
					if (selected < 0)
					selected = 16 + selected;
					selected = selected % 16;
					printNumpadButton(selected, 1);
					break;

					case (3):
					selected += 1;
					if (selected < 0)
					selected = 16 + selected;
					selected = selected % 16;
					printNumpadButton(selected, 1);

					break;

					case (4):
					printNumpadButton(selected, 1);
					switch (selected) {
						case (0):
						if (number < 0)
						number = number * 10 - 1;
						else
						number = number * 10 + 1;
						break;
						case (1):
						if (number < 0)
						number = number * 10 - 2;
						else
						number = number * 10 + 2;
						break;
						case (2):
						if (number < 0)
						number = number * 10 - 3;
						else
						number = number * 10 + 3;
						break;
						case (3):
						number = number / 10;
						break;
						case (4):
						if (number < 0)
						number = number * 10 - 4;
						else
						number = number * 10 + 4;
						break;
						case (5):
						if (number < 0)
						number = number * 10 - 5;
						else
						number = number * 10 + 5;
						break;
						case (6):
						if (number < 0)
						number = number * 10 - 6;
						else
						number = number * 10 + 6;
						break;
						case (7):
						number = 0;
						break;
						case (8):
						if (number < 0)
						number = number * 10 - 7;
						else
						number = number * 10 + 7;
						break;
						case (9):
						if (number < 0)
						number = number * 10 - 8;
						else
						number = number * 10 + 8;
						break;
						case (10):
						if (number < 0)
						number = number * 10 - 9;
						else
						number = number * 10 + 9;
						break;
						case (11):
						// SerialUSB.println("returning");
						return number;
						case (12):
						break;
						case (13):
						number = number * 10;
						break;
						case (14):
						number = -number;
						break;
						case (15):
						SerialUSB.println(initial_number);
						return initial_number;
					}
					display.fillRect(NUMPAD_LEFT_OFFSET + 1, NUMPAD_BOTTOM_OFFSET - 4 * NUMPAD_BUTTON_SIZE - 39, 4 * NUMPAD_BUTTON_SIZE - 1, 38, GxEPD_WHITE);
					display.setCursor(NUMPAD_LEFT_OFFSET + 12, NUMPAD_BOTTOM_OFFSET - 5 * NUMPAD_BUTTON_SIZE + 25);
					display.print(number);
					break;
				}
			}
			display.display(true);
		}
	}

}

void numpadInit(int number) {

	display.fillRect(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_WHITE);

	display.writeFastVLine(NUMPAD_LEFT_OFFSET + 0 * NUMPAD_BUTTON_SIZE,       NUMPAD_BOTTOM_OFFSET - 4 * NUMPAD_BUTTON_SIZE - 40,    4 * NUMPAD_BUTTON_SIZE + 40, GxEPD_BLACK);
	display.writeFastVLine(NUMPAD_LEFT_OFFSET - 1 + 0 * NUMPAD_BUTTON_SIZE,   NUMPAD_BOTTOM_OFFSET - 4 * NUMPAD_BUTTON_SIZE - 40,    4 * NUMPAD_BUTTON_SIZE + 40, GxEPD_BLACK);
	display.writeFastVLine(NUMPAD_LEFT_OFFSET + 1 * NUMPAD_BUTTON_SIZE,       NUMPAD_BOTTOM_OFFSET - 4 * NUMPAD_BUTTON_SIZE,         4 * NUMPAD_BUTTON_SIZE, GxEPD_BLACK);
	display.writeFastVLine(NUMPAD_LEFT_OFFSET + 2 * NUMPAD_BUTTON_SIZE,       NUMPAD_BOTTOM_OFFSET - 4 * NUMPAD_BUTTON_SIZE,         4 * NUMPAD_BUTTON_SIZE, GxEPD_BLACK);
	display.writeFastVLine(NUMPAD_LEFT_OFFSET + 3 * NUMPAD_BUTTON_SIZE,       NUMPAD_BOTTOM_OFFSET - 4 * NUMPAD_BUTTON_SIZE,         4 * NUMPAD_BUTTON_SIZE, GxEPD_BLACK);
	display.writeFastVLine(NUMPAD_LEFT_OFFSET + 4 * NUMPAD_BUTTON_SIZE,       NUMPAD_BOTTOM_OFFSET - 4 * NUMPAD_BUTTON_SIZE - 40,    4 * NUMPAD_BUTTON_SIZE + 40, GxEPD_BLACK);
	display.writeFastVLine(NUMPAD_LEFT_OFFSET + 1 + 4 * NUMPAD_BUTTON_SIZE,   NUMPAD_BOTTOM_OFFSET - 4 * NUMPAD_BUTTON_SIZE - 40,    4 * NUMPAD_BUTTON_SIZE + 40, GxEPD_BLACK);


	display.writeFastHLine(NUMPAD_LEFT_OFFSET, NUMPAD_BOTTOM_OFFSET - 4 * NUMPAD_BUTTON_SIZE - 1, 4 * NUMPAD_BUTTON_SIZE, GxEPD_BLACK);
	display.writeFastHLine(NUMPAD_LEFT_OFFSET, NUMPAD_BOTTOM_OFFSET - 4 * NUMPAD_BUTTON_SIZE - 40, 4 * NUMPAD_BUTTON_SIZE, GxEPD_BLACK);
	display.writeFastHLine(NUMPAD_LEFT_OFFSET, NUMPAD_BOTTOM_OFFSET - 4 * NUMPAD_BUTTON_SIZE - 41, 4 * NUMPAD_BUTTON_SIZE, GxEPD_BLACK);
	display.writeFastHLine(NUMPAD_LEFT_OFFSET, NUMPAD_BOTTOM_OFFSET - 4 * NUMPAD_BUTTON_SIZE, 4 * NUMPAD_BUTTON_SIZE, GxEPD_BLACK);
	display.writeFastHLine(NUMPAD_LEFT_OFFSET, NUMPAD_BOTTOM_OFFSET - 3 * NUMPAD_BUTTON_SIZE, 4 * NUMPAD_BUTTON_SIZE, GxEPD_BLACK);
	display.writeFastHLine(NUMPAD_LEFT_OFFSET, NUMPAD_BOTTOM_OFFSET - 2 * NUMPAD_BUTTON_SIZE, 4 * NUMPAD_BUTTON_SIZE, GxEPD_BLACK);
	display.writeFastHLine(NUMPAD_LEFT_OFFSET, NUMPAD_BOTTOM_OFFSET - 1 * NUMPAD_BUTTON_SIZE, 4 * NUMPAD_BUTTON_SIZE, GxEPD_BLACK);
	display.writeFastHLine(NUMPAD_LEFT_OFFSET, NUMPAD_BOTTOM_OFFSET, 4 * NUMPAD_BUTTON_SIZE, GxEPD_BLACK);


	display.setCursor(NUMPAD_LEFT_OFFSET + 12, NUMPAD_BOTTOM_OFFSET - 5 * NUMPAD_BUTTON_SIZE + 25);
	display.print(number);

	//display.update();
	for (int i = 0; i < 16; i++) {
		printNumpadButton(i, 0);

	}

	printNumpadButton(0, 1);
	display.display(true);
}

void printNumpadButton(int index, int pressed) {
	//display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, false);
	int background = GxEPD_WHITE;
	int text = GxEPD_BLACK;
	if (pressed == 1) {
		text = GxEPD_WHITE;
		background = GxEPD_BLACK;
	}
	display.setTextColor(text);

	display.fillRect(NUMPAD_LEFT_OFFSET + 1 + (index % 4) * NUMPAD_BUTTON_SIZE, NUMPAD_BOTTOM_OFFSET - 4 * NUMPAD_BUTTON_SIZE + 1 + index / 4 * NUMPAD_BUTTON_SIZE , NUMPAD_BUTTON_SIZE - 1, NUMPAD_BUTTON_SIZE - 1, background);
	switch (index) {

		case (0):
		display.setCursor(NUMPAD_LEFT_OFFSET + 12 + index % 4 * NUMPAD_BUTTON_SIZE, NUMPAD_BOTTOM_OFFSET - 4 * NUMPAD_BUTTON_SIZE + 25 + index / 4 * NUMPAD_BUTTON_SIZE);
		display.print("1");
		break;
		case (1):
		display.setCursor(NUMPAD_LEFT_OFFSET + 12 + index % 4 * NUMPAD_BUTTON_SIZE, NUMPAD_BOTTOM_OFFSET - 4 * NUMPAD_BUTTON_SIZE + 25);
		display.print("2");
		break;
		case (2):
		display.setCursor(NUMPAD_LEFT_OFFSET + 12 + index % 4 * NUMPAD_BUTTON_SIZE, NUMPAD_BOTTOM_OFFSET - 4 * NUMPAD_BUTTON_SIZE + 25);
		display.print("3");
		break;
		case (3):
		display.setCursor(NUMPAD_LEFT_OFFSET + 10 + index % 4 * NUMPAD_BUTTON_SIZE, NUMPAD_BOTTOM_OFFSET - 4 * NUMPAD_BUTTON_SIZE + 25 + index / 4 * NUMPAD_BUTTON_SIZE);
		display.print("<");
		display.setCursor(NUMPAD_LEFT_OFFSET + 14 + index % 4 * NUMPAD_BUTTON_SIZE, NUMPAD_BOTTOM_OFFSET - 4 * NUMPAD_BUTTON_SIZE + 25 + index / 4 * NUMPAD_BUTTON_SIZE);
		display.print("-");
		break;
		case (4):    display.setCursor(NUMPAD_LEFT_OFFSET + 12 + index % 4 * NUMPAD_BUTTON_SIZE, NUMPAD_BOTTOM_OFFSET - 4 * NUMPAD_BUTTON_SIZE + 25 + index / 4 * NUMPAD_BUTTON_SIZE);
		display.print("4");
		break;
		case (5):
		display.setCursor(NUMPAD_LEFT_OFFSET + 12 + index % 4 * NUMPAD_BUTTON_SIZE, NUMPAD_BOTTOM_OFFSET - 4 * NUMPAD_BUTTON_SIZE + 25 + index / 4 * NUMPAD_BUTTON_SIZE);
		display.print("5");
		break;
		case (6):
		display.setCursor(NUMPAD_LEFT_OFFSET + 12 + index % 4 * NUMPAD_BUTTON_SIZE, NUMPAD_BOTTOM_OFFSET - 4 * NUMPAD_BUTTON_SIZE + 25 + index / 4 * NUMPAD_BUTTON_SIZE);
		display.print("6");
		break;
		case (7):
		display.setCursor(NUMPAD_LEFT_OFFSET + 12 + index % 4 * NUMPAD_BUTTON_SIZE, NUMPAD_BOTTOM_OFFSET - 4 * NUMPAD_BUTTON_SIZE + 25 + index / 4 * NUMPAD_BUTTON_SIZE);
		display.print("C");
		break;
		case (8):
		display.setCursor(NUMPAD_LEFT_OFFSET + 12 + index % 4 * NUMPAD_BUTTON_SIZE, NUMPAD_BOTTOM_OFFSET - 4 * NUMPAD_BUTTON_SIZE + 25 + index / 4 * NUMPAD_BUTTON_SIZE);
		display.print("7");
		break;
		case (9):
		display.setCursor(NUMPAD_LEFT_OFFSET + 12 + index % 4 * NUMPAD_BUTTON_SIZE, NUMPAD_BOTTOM_OFFSET - 4 * NUMPAD_BUTTON_SIZE + 25 + index / 4 * NUMPAD_BUTTON_SIZE);
		display.print("8");
		break;
		case (10):
		display.setCursor(NUMPAD_LEFT_OFFSET + 12 + index % 4 * NUMPAD_BUTTON_SIZE, NUMPAD_BOTTOM_OFFSET - 4 * NUMPAD_BUTTON_SIZE + 25 + index / 4 * NUMPAD_BUTTON_SIZE);
		display.print("9");
		break;
		case (11):
		//   display.fillRect(143, 40 + (index / 4) * 40 , 39, 39, background);
		display.setCursor(NUMPAD_LEFT_OFFSET + 12 + index % 4 * NUMPAD_BUTTON_SIZE - 7, NUMPAD_BOTTOM_OFFSET - 4 * NUMPAD_BUTTON_SIZE + 25 + index / 4 * NUMPAD_BUTTON_SIZE);
		display.print("ok");
		break;
		case (12):
		display.setCursor(NUMPAD_LEFT_OFFSET + 12 + index % 4 * NUMPAD_BUTTON_SIZE, NUMPAD_BOTTOM_OFFSET - 4 * NUMPAD_BUTTON_SIZE + 25 + index / 4 * NUMPAD_BUTTON_SIZE);
		display.print(",");
		break;
		case (13):

		display.setCursor(NUMPAD_LEFT_OFFSET + 12 + index % 4 * NUMPAD_BUTTON_SIZE, NUMPAD_BOTTOM_OFFSET - 4 * NUMPAD_BUTTON_SIZE + 25 + index / 4 * NUMPAD_BUTTON_SIZE);
		display.print("0");
		break;
		case (14):
		display.setCursor(NUMPAD_LEFT_OFFSET + 5 + index % 4 * NUMPAD_BUTTON_SIZE, NUMPAD_BOTTOM_OFFSET - 4 * NUMPAD_BUTTON_SIZE + 18 + index / 4 * NUMPAD_BUTTON_SIZE);
		display.print("+");
		display.setCursor(NUMPAD_LEFT_OFFSET + 12 + index % 4 * NUMPAD_BUTTON_SIZE, NUMPAD_BOTTOM_OFFSET - 4 * NUMPAD_BUTTON_SIZE + 25 + index / 4 * NUMPAD_BUTTON_SIZE);
		display.print("/");
		display.setCursor(NUMPAD_LEFT_OFFSET + 20 + index % 4 * NUMPAD_BUTTON_SIZE, NUMPAD_BOTTOM_OFFSET - 4 * NUMPAD_BUTTON_SIZE + 30 + index / 4 * NUMPAD_BUTTON_SIZE);
		display.print("-");

		break;
		case (15):
		display.setCursor(NUMPAD_LEFT_OFFSET + 12 + index % 4 * NUMPAD_BUTTON_SIZE, NUMPAD_BOTTOM_OFFSET - 4 * NUMPAD_BUTTON_SIZE + 25 + index / 4 * NUMPAD_BUTTON_SIZE);
		display.print("x");
		break;
	}
	display.setTextColor(GxEPD_BLACK);
}


void keypad(char* dest, int leng){
	//char stringer[leng+1] = "";
	//dest = "";
	int selected = 0;
	int current = 0;
	display.fillScreen(GxEPD_WHITE);
	display.writeFastHLine(0, 25, 200, GxEPD_BLACK);
	display.setFont(&FreeMonoBold9pt7b);
	for(int i = 0; i < 100; i++) {
		display.setCursor((i%10) * 20+3, (i/10)*17+40);
		if(i == 96)	{	
			display.setCursor((i%10) * 20, (i/10)*17+40);
			display.print("ok");
		}
		else{
		display.write(i+32);
		}
	}
	
				display.fillRect(0, 0, 200, 24, GxEPD_WHITE);
				display.fillRect(20*(selected%10), 26 + (selected/10)*17, 20, 17, GxEPD_BLACK);
				display.setCursor((selected%10) * 20+3, (selected/10)*17+40);
				display.setTextColor(GxEPD_WHITE);
				display.write(selected+32);
				display.setTextColor(GxEPD_BLACK);
				display.setCursor(12, 15);
				display.print(dest);
				display.display(1);

	do{
		if(buttons.getFlag()){
			display.fillRect(20*(selected%10), 26 + (selected/10)*17, 20, 17, GxEPD_WHITE);
			display.setCursor((selected%10) * 20+3, (selected/10)*17+40);
					if(selected == 96)	{
						display.setCursor((selected%10) * 20, (selected/10)*17+40);
						display.print("ok");
					}
					else{
						display.write(selected+32);
					}
					
			switch (buttons.getButtonPressed()) {
				case (UP):
				selected -= 10;
				if (selected < 0)
				selected +=100;
				break;
				
				case (DOWN):
				selected += 10;
				if (selected > 99)
				selected = selected%100;
				break;
				
				case (LEFT):
				selected -= 1;
				if (selected < 0)
				selected +=100;
				break;
				
				case (RIGHT):
				selected += 1;
				if (selected > 99)
				selected = selected - 100;
				break;
				
				case (PRESS):
				if(selected == 96) {
					display.setFont(&FreeMonoBold12pt7b);
					return;
				
				}
				if(current < leng){		
					dest[current] = selected+32;			
					dest[current+1] = '\0';
					current++;
				}
				else {display.setFont(&FreeMonoBold12pt7b);
					return;
					}
				break;

				
				
				
			}
			display.fillRect(0, 0, 200, 24, GxEPD_WHITE);
			display.fillRect(20*(selected%10), 26 + (selected/10)*17, 20, 17, GxEPD_BLACK);
			display.setCursor((selected%10) * 20+3, (selected/10)*17+40);
			display.setTextColor(GxEPD_WHITE);
								if(selected == 96)	{
									display.setCursor((selected%10) * 20, (selected/10)*17+40);
									display.print("ok");
								}
								else{
									display.write(selected+32);
								}
			display.setTextColor(GxEPD_BLACK);
			display.setCursor(12, 15);
			display.print(dest);
			display.display(1);
		}
	}while(1);
}