/*
 * Menu.h
 *
 * Created: 6/10/2018 0:15:35
 *  Author: Jena
 */ 


#ifndef MENU_H_
#define MENU_H_

typedef struct {
	char (*pole)[15];
	char *jmeno_menu;
	int velikost;
	int menu_id;
	int selected = 0;
	int selected_old = 0;
	bool is_detailed = false;
} menu;


void menu_init(void);

void MenuEntry(menu *menuPointer);

void drawMenu(menu *menuPointer);

void drawMenuDetailed(menu *menuPointer);

void drawMenuSimple(menu *menuPointer);

void menuSelector(menu *menuPointer, int selected);

void debugBMI160(void);
void debugBME280(void);
void debugMAX17055(void);
void debugGPS(void);
void debugXOSC32k(void);
void printAccelerometerData(void);

extern menu topmenu;
extern menu menu0;
extern menu menu1;
extern menu menu2;
extern menu menu3;
extern menu system_menu;


extern double myRealAltitude;





#endif /* MENU_H_ */