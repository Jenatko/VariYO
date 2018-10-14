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
} menu;


void menu_init(void);

void drawMenu(menu *menuPointer);

void drawMenuDetailed(menu *menuPointer);

void menuSelector(menu *menuPointer, int selected);



extern menu topmenu;
extern menu menu0;
extern menu menu1;
extern menu menu2;
extern menu menu3;
extern menu menu4;

extern int th_rise;
extern int th_sink;
extern double myRealAltitude;

extern int tracklog_ena;


#endif /* MENU_H_ */