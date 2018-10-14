#include <GxGDEP015OC1/GxGDEP015OC1.h>
#include <Fonts/FreeMonoBold24pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>

#include "Menu.h"
#include "button_control.h"
#include "numpad.h"
#include "roundbuff.h"
#include "MS5611.h"
#include "powerModes.h"


char listTopMenu[][15] = {"0. Peripher", "1. Altimeter", "2. Vario", "3. Tracklog", "MassStorage1", "Power Off"};
char topMenuName[15] = "TopMenu";
int topMenuID = 0x01;

char menu0_list[][15] = {"0.0 Altimeter", "0.1 Buzzer", "0.2 GPS", "0.3 Tracklog", "0.4 Compass"};
char menu0_name[15] = "Periph";
int  menu0_id = 0x10;

char menu1_list[][15] = {"1.0 set Alt", "1.1 set SeaP", "1.2 item"};
char menu1_name[15] = "Altimeter";
int  menu1_id = 0x11;

char menu2_list[][15] = {"2.0 Thres. up", "2.1 Thresh. dn", "Equation rise", "Equation sink", "rise_data", "sink_data", "", ""};
char menu2_name[15] = "Vario";
int  menu2_id = 0x12;

char menu3_list[][15] = {"Start", "Stop", "3.2 item", "3.3 item", "3.4 item", "3.5 item"};
char menu3_name[15] = "3. Tracklog";
int  menu3_id = 0x13;

char menu4_list[][15] = {"4.0 item", "4.1 item", "4.2 item", "4.3 item", "4.4 item", "4.5 item"};
char menu4_name[15] = "4item Menu";
int  menu4_id = 0x14;


menu topmenu;
menu menu0;
menu menu1;
menu menu2;
menu menu3;
menu menu4;


void menu_init() {


	topmenu.pole = listTopMenu;
	topmenu.velikost = sizeof(listTopMenu) / sizeof(listTopMenu[0]);
	topmenu.jmeno_menu = topMenuName;
	topmenu.menu_id = topMenuID;

	menu0.pole = menu0_list;
	menu0.velikost = sizeof(menu0_list) / sizeof(menu0_list[0]);
	menu0.jmeno_menu = menu0_name;
	menu0.menu_id = menu0_id;

	menu1.pole = menu1_list;
	menu1.velikost = sizeof(menu1_list) / sizeof(menu1_list[0]);
	menu1.jmeno_menu = menu1_name;
	menu1.menu_id = menu1_id;

	menu2.pole = menu2_list;
	String tmpstring =  String((float)th_rise / 100);
	tmpstring.concat(" m/s");
	tmpstring.toCharArray(menu2.pole[4], 15);

	tmpstring =  String((float)th_sink / 100);
	tmpstring.concat(" m/s");
	tmpstring.toCharArray(menu2.pole[5], 15);

	menu2.velikost = sizeof(menu2_list) / sizeof(menu2_list[0]);
	menu2.jmeno_menu = menu2_name;
	menu2.menu_id = menu2_id;

	menu3.pole = menu3_list;
	menu3.velikost = sizeof(menu3_list) / sizeof(menu3_list[0]);
	menu3.jmeno_menu = menu3_name;
	menu3.menu_id = menu3_id;

	menu4.pole = menu4_list;
	menu4.velikost = sizeof(menu4_list) / sizeof(menu4_list[0]);
	menu4.jmeno_menu = menu4_name;
	menu4.menu_id = menu4_id;


}






void drawMenu(menu *menuPointer) {
	int selected = 0;
	redraw:
	display.fillRect(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_WHITE);
	display.fillRect(0, 102, GxEPD_WIDTH, 32, GxEPD_BLACK);
	display.writeFastHLine(0, 30, 200, GxEPD_BLACK);
	display.writeFastHLine(0, 31, 200, GxEPD_BLACK);
	display.writeFastHLine(0, 32, 200, GxEPD_BLACK);
	display.setCursor(10, 20);
	display.print(menuPointer->jmeno_menu);


	do {
		for (int i = 0; i < 5; i++) {

			int temp = (selected - 2 + i) % menuPointer->velikost;
			if (temp < 0)
			temp = menuPointer->velikost + temp;
			if (i == 2) {

				display.setTextColor(GxEPD_WHITE);
				display.setCursor(10, 60 + 32 * i);
				display.print(menuPointer->pole[temp]);
				display.setTextColor(GxEPD_BLACK);
				continue;

			}

			display.setCursor(10, 60 + 32 * i);
			display.print(menuPointer->pole[temp]);
			// display.print(temp);

		}
		display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, false);



		while (!buttons.getFlag()) {
			buttons.buttonUpdate();
		}
		display.fillRect(0, 33, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_WHITE);
		display.fillRect(0, 102, GxEPD_WIDTH, 32, GxEPD_BLACK);

		if (buttons.getFlag()) {
			int button = buttons.getButtonPressed();
			if (button == UP) {
				selected -= 1;
				if (selected < 0)
				selected = menuPointer->velikost + selected;
			}
			else if (button == DOWN) {
				selected += 1;
				selected = + selected % menuPointer->velikost;
			}
			else if (button == LEFT) {
				break;
			}
			else if (button == RIGHT || button == PRESS) {
				menuSelector(menuPointer, selected);
				goto redraw;
			}
		}
	}
	while (1);
}






void drawMenuDetailed(menu *menuPointer) {
	int selected = 0;
	redraw:
	display.fillRect(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_WHITE);
	display.fillRect(0, 90, GxEPD_WIDTH, 58, GxEPD_BLACK);
	display.writeFastHLine(0, 30, 200, GxEPD_BLACK);
	display.writeFastHLine(0, 31, 200, GxEPD_BLACK);
	display.writeFastHLine(0, 32, 200, GxEPD_BLACK);
	display.setCursor(10, 20);
	display.print(menuPointer->jmeno_menu);


	do {
		for (int i = 0; i < 6; i++) {

			int temp = (selected - 1 + i) % ((menuPointer->velikost) / 2);
			if (temp < 0)
			temp = (menuPointer->velikost) / 2 + temp;
			if (i < 3) {
				if (i == 1 ) {

					display.setTextColor(GxEPD_WHITE);
					display.setCursor(10, 58 + 58 * i);
					display.print(menuPointer->pole[temp]);
					display.setTextColor(GxEPD_BLACK);

					display.setFont(&FreeMonoBold9pt7b);
					display.setTextColor(GxEPD_WHITE);
					display.setCursor(80, 78 + 58 * (i));
					display.print(menuPointer->pole[temp + (menuPointer->velikost) / 2]);
					display.setTextColor(GxEPD_BLACK);
					display.setFont(&FreeMonoBold12pt7b);
					continue;

				}

				display.setCursor(10, 58 + 58 * i);
				display.print(menuPointer->pole[temp]);

				display.setFont(&FreeMonoBold9pt7b);
				display.setCursor(80, 78 + 58 * (i));
				display.print(menuPointer->pole[temp + (menuPointer->velikost) / 2]);
				// display.print(temp);
				display.setFont(&FreeMonoBold12pt7b);
			}
		}

		display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, false);



		while (!buttons.getFlag()) {
			buttons.buttonUpdate();
		}
		display.fillRect(0, 33, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_WHITE);
		display.fillRect(0, 90, GxEPD_WIDTH, 58, GxEPD_BLACK);

		if (buttons.getFlag()) {
			int button = buttons.getButtonPressed();
			if (button == 0) {
				selected -= 1;
				if (selected < 0)
				selected = menuPointer->velikost + selected;
			}
			else if (button == 1) {
				selected += 1;
				selected = + selected % menuPointer->velikost;
			}
			else if (button == 2) {
				break;
			}
			else if (button == 3 || button == 4) {
				menuSelector(menuPointer, selected);
				goto redraw;
			}
		}
	}
	while (1);
}







void menuSelector(menu *menuPointer, int selected) {
	
	
	//Top menu
	if (menuPointer->menu_id == 0x01)
	if (selected == 0)
	drawMenu(&menu0);
	else if (selected == 1)
	drawMenu(&menu1);
	else if (selected == 2)
	drawMenuDetailed(&menu2);
	else if (selected == 3)
	drawMenu(&menu3);
	else if (selected == 4)
	//drawMenu(&menu4);
	massStorageEna();
	//power off
	else if (selected == 5) {
		powerOff();
		buttons.getButtonPressed();
	}
	//Altimeter
	if (menuPointer->menu_id == 0x11) {
		if (selected == 0)
		ms5611.setSeaLevel(numpad(myRealAltitude), rbuff.getAverage());
		else if (selected == 1)
		ms5611.putSeaLevel(numpad(500));
		// ms5611.putSeaLevel(numpad((int)ms5611.readSeaPressure));
	}

	//Vario
	if (menuPointer->menu_id == 0x12) {
		if (selected == 0) {
			th_rise = numpad(th_rise);
			if (th_rise < 0)
			th_rise *= -1;
			menu_init() ;
		}
		else if (selected == 1) {
			th_sink = numpad(th_sink);
			if (th_sink > 0)
			th_sink *= -1;

			menu_init() ;
		}
		// ms5611.putSeaLevel(numpad((int)ms5611.readSeaPressure));
	}
	//Tracklog
	if (menuPointer->menu_id == 0x13) {
		//start
		if (selected == 0){
		tracklog_ena = 1;
		SerialUSB.println("ena=1")		;
		}
		//stop
		else if (selected == 1){
		tracklog_ena = 0;
		SerialUSB.println("ena=0");
		}
		
		
		// ms5611.putSeaLevel(numpad((int)ms5611.readSeaPressure));
	}
	

}
