/*
 * Menu.h
 *
 * Created: 6/10/2018 0:15:35
 *  Author: Jena
 */ 


#ifndef MENU_H_
#define MENU_H_

#include "Variables.h"

typedef struct {
	char (*items_array)[15];
	char *menu_name;
	int no_items;
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

void debugBMX160(void);
void debugBME280(void);
void debugMAX17055(void);
void debugGPS(void);
void debugXOSC32k(void);
void printAccelerometerData(void);

extern menu topmenu;
extern menu utilities_menu;
extern menu altimemter_menu;
extern menu vario_menu;
extern menu debug_menu;
extern menu settings_menu;
extern menu gauge_menu;
extern menu gauges_menu;
extern menu logger_menu;

void Gauge_print(Gauge *gau);
void setGaugeMenu(Gauge *gaugePointer);
void printGauges();

void calibrateMagnetometer();
void calibrateAccelerometer();

void showFlightSummary(void);

extern double myRealAltitude;





#endif /* MENU_H_ */