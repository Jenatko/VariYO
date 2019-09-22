
#include <GxEPD2_BW.h>
//#include <GxGDEP015OC1/GxGDEP015OC1.h>
#include <Fonts/FreeMonoBold24pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>

#include "Menu.h"
#include "button_control.h"
#include "numpad.h"
#include "roundbuff.h"
#include "powerModes.h"
#include "routine.h"
#include "routine.h"
#include "MAX17055.h"
#include "Variables.h"
#include "EEPROM.h"
#include "MEMS.h"
#include "RTCZero.h"
#include "buzzer.h"
#include "wiring_private.h"
#include "GPSfix.h"
#include "chess.h"
#include "kalmanfilter3.h"
#include "Interrupt_counter.h"

#include "MadgwickAHRS.h"
#include "elipsoid.h"
#include "compass.h"

#include "logger.h"


#define MENUID_TOPMENU 0x01
#define MENUITEM_TOPMENU_SETINGS 0
#define MENUITEM_TOPMENU_UTILITIES 1
#define MENUITEM_TOPMENU_DEBUG 2
#define MENUITEM_TOPMENU_START_TRACKLOG 3
#define MENUITEM_TOPMENU_MASS_STORAGE 4
#define MENUITEM_TOPMENU_POWEROFF 5

char listTopMenu[][15] = {"Settings", "Utilities", "Debug", "Start Trcklog", "MassStorage", "Power Off"};
char topMenuName[15] = "TopMenu";
int topMenuID = MENUID_TOPMENU;

#define MENUID_UTILITIES 0x02
#define MENUITEM_UTILITIES_COMPASS 0
#define MENUITEM_UTILITIES_CHESS 1
#define MENUITEM_UTILITIES_LOGGER 2

char menu0_list[][15] = {"Compass","Chess", "Logger"};
char menu0_name[15] = "Utilities";
int  menu0_id = MENUID_UTILITIES;

#define MENUID_LOGGER 0x09
#define MENUITEM_LOGGER_MINUTE 0
#define MENUITEM_LOGGER_HOUR 1
#define MENUITEM_LOGGER_DAY 2

char menulog_list[][15] = {"Every minute","Every hour", "Every day"};
char menulog_name[15] = "Logger";
int  menulog_id = MENUID_LOGGER;

#define MENUID_ALTIMETER 0x03
#define MENUITEM_ALTIMETER_SET_ALT 0
#define MENUITEM_ALTIMETER_SET_SEAP 1
#define MENUITEM_ALTIMETER_ALT_FROM_GPS 2
#define MENUITEM_ALTIMETER_ALT_FROM_HGT 3

char menu1_list[][15] = {"set Alt", "set SeaP", "Alt from GPS", "Alt from .hgt"};
char menu1_name[15] = "Altimeter";
int  menu1_id = MENUID_ALTIMETER;


#define MENUID_VARIO 0x04
#define MENUITEM_VARIO_THRESHOLD_UP 0
#define MENUITEM_VARIO_THRESHOLD_DOWN 1
#define MENUITEM_VARIO_EQ_RISE 2
#define MENUITEM_VARIO_EQ_SINK 3
#define MENUITEM_VARIO_ZVAR 4
#define MENUITEM_VARIO_ACCELVAR 5
#define MENUITEM_VARIO_AVERAGING 6
#define MENUITEM_VARIO_DATA_RISE 7
#define MENUITEM_VARIO_DATA_SINK 8
#define MENUITEM_VARIO_DATA_ZVAR 11
#define MENUITEM_VARIO_DATA_ACCVAR 12
#define MENUITEM_VARIO_DATA_AVERAGING 13


char menu2_list[][15] = {"Thres. up", "Thresh. dn", "Equation rise", "Equation sink", "Zvar", "accelvar", "averaging", "rise_data", "sink_data","","","","", ""};
char menu2_name[15] = "Vario";
int  menu2_id = MENUID_VARIO;


#define MENUID_DEBUG 0x05
#define MENUITEM_DEBUG_BMX160 0
#define MENUITEM_DEBUG_LPS33_SI7021 1
#define MENUITEM_DEBUG_MAX17055 2
#define MENUITEM_DEBUG_GPS 3
#define MENUITEM_DEBUG_PRINT_EEPROM 4
#define MENUITEM_DEBUG_32KHZ_TEST 5
#define MENUITEM_DEBUG_PRINT_ACCEL 6


char menu3_list[][15] = {"BMX160", "LPS33,SI7021", "MAX17055", "GPS", "print EEPROM", "32kHz test", "Print accel"};
char menu3_name[15] = "Debug";
int  menu3_id = MENUID_DEBUG;



#define MENUID_SETTINGS 0x06
#define MENUITEM_SETTINGS_ALTIMETER 0
#define MENUITEM_SETTINGS_VARIO 1
#define MENUITEM_SETTINGS_GPS_POWER 2
#define MENUITEM_SETTINGS_BUZZER 3
#define MENUITEM_SETTINGS_TIME_ZONE 4
#define MENUITEM_SETTINGS_SAVE_EEPROM 5
#define MENUITEM_SETTINGS_SET_DEFAULT 6
#define MENUITEM_SETTINGS_HEATER 7
#define MENUITEM_SETTINGS_GAUGES 8
#define MENUITEM_SETTINGS_CALIB_ACCEL 9
#define MENUITEM_SETTINGS_CALIB_MAG 10

char system_menu_list[][15] = {"Altimeter", "Vario", "GPS power", "buzzer","Time zone", "saveEEPROM", "set default", "heater off", "Gauges", "calib Accel", "calib mag"};
char system_menu_name[15] = "Settings";
int  system_menu_id = MENUID_SETTINGS;


#define MENUID_GAUGE 0x07
#define MENUITEM_GAUGE_ENABLE 0
#define MENUITEM_GAUGE_POSITION 1
#define MENUITEM_GAUGE_SIZE 2
#define MENUITEM_GAUGE_FRAME 3
#define MENUITEM_GAUGE_DECIMALS 4
#define MENUITEM_GAUGE_NAME 5
#define MENUITEM_GAUGE_UNITS 6
#define MENUITEM_GAUGE_FONT 7
#define MENUITEM_GAUGE_PLUS_SIGN 8
#define MENUITEM_GAUGE_FIXED_DECIMALS 9
#define MENUITEM_GAUGE_AVERAGING 10

char gauge_menu_list[][15] = {"enable", "position", "size", "frame", "decimals", "name", "units", "font", "Showing + sgn", "fixed decimal", "averaging"};
char gauge_menu_name[15] = "Gauge";
int  gauge_menu_id = MENUID_GAUGE;


#define MENUID_GAUGES 0x08
#define MENUITEM_GAUGES_VARIO 0
#define MENUITEM_GAUGES_VARIO_AVG 1
#define MENUITEM_GAUGES_ALTITUDE 2
#define MENUITEM_GAUGES_AGL 3
#define MENUITEM_GAUGES_SPEED 4
#define MENUITEM_GAUGES_HEADING 5
#define MENUITEM_GAUGES_WIND_SPEED 6
#define MENUITEM_GAUGES_WIND_DIR 7
#define MENUITEM_GAUGES_TEMPERATURE 8
#define MENUITEM_GAUGES_HUMIDITY 9
#define MENUITEM_GAUGES_FLIGHT_TIME 10
#define MENUITEM_GAUGES_LIFT_TO_DRAG 11
#define MENUITEM_GAUGES_ALT_ABOVE_TAKEOFF 12


char gauges_menu_list[][15] = {"Vario", "Vario avg", "Altitude", "AGL", "Speed", "Heading", "Wind speed", "Wind dir", "Temperature", "Humidity", "Flight time", "Lift-to-drag", "Alt above TO"};
char gauges_menu_name[15] = "Gauges";
int  gauges_menu_id = MENUID_GAUGES;




int lastmenutype = 0;


menu topmenu;
menu utilities_menu;
menu altimemter_menu;
menu vario_menu;
menu debug_menu;
menu settings_menu;
menu gauge_menu;
menu gauges_menu;
menu logger_menu;

Gauge *gaugepointer = &statVar.varioGauge;


void menu_init() {


	topmenu.items_array = listTopMenu;
	topmenu.no_items = sizeof(listTopMenu) / sizeof(listTopMenu[0]);
	topmenu.menu_name = topMenuName;
	topmenu.menu_id = topMenuID;

	utilities_menu.items_array = menu0_list;
	utilities_menu.no_items = sizeof(menu0_list) / sizeof(menu0_list[0]);
	utilities_menu.menu_name = menu0_name;
	utilities_menu.menu_id = menu0_id;
	
	logger_menu.items_array = menulog_list;
	logger_menu.no_items = sizeof(menulog_list) / sizeof(menulog_list[0]);
	logger_menu.menu_name = menulog_name;
	logger_menu.menu_id = menulog_id;

	altimemter_menu.items_array = menu1_list;
	altimemter_menu.no_items = sizeof(menu1_list) / sizeof(menu1_list[0]);
	altimemter_menu.menu_name = menu1_name;
	altimemter_menu.menu_id = menu1_id;

	vario_menu.items_array = menu2_list;
	vario_menu.is_detailed = true;
	String tmpstring =  String((float)statVar.th_rise / 100);
	tmpstring.concat(" m/s");
	tmpstring.toCharArray(vario_menu.items_array[MENUITEM_VARIO_DATA_RISE], 15);

	tmpstring =  String((float)statVar.th_sink / 100);
	tmpstring.concat(" m/s");
	tmpstring.toCharArray(vario_menu.items_array[MENUITEM_VARIO_DATA_SINK], 15);
	
	vario_menu.items_array = menu2_list;
	vario_menu.is_detailed = true;
	tmpstring =  String((float)statVar.zvariance);
	tmpstring.toCharArray(vario_menu.items_array[MENUITEM_VARIO_DATA_ZVAR], 15);

	tmpstring =  String((float)statVar.accelvariance);
	tmpstring.toCharArray(vario_menu.items_array[MENUITEM_VARIO_DATA_ACCVAR], 15);
	
	tmpstring =  String((float)statVar.vario_lowpass_coef);
	tmpstring.toCharArray(vario_menu.items_array[MENUITEM_VARIO_DATA_AVERAGING], 15);
	
	if(statVar.ena_vector&ENA_GPS){
		if(statVar.ena_vector&ENA_GPS_LOW_POWER)
		strncpy(system_menu_list[MENUITEM_SETTINGS_GPS_POWER], "GPS low", 15);
		else strncpy(system_menu_list[MENUITEM_SETTINGS_GPS_POWER], "GPS full", 15);
	}
	else strncpy(system_menu_list[MENUITEM_SETTINGS_GPS_POWER], "GPS Off", 15);
	
	
	if(statVar.ena_vector&ENA_BUZZER && statVar.ena_vector&ENA_BUZZ_WHEN_LOGGING) strncpy(system_menu_list[MENUITEM_SETTINGS_BUZZER], "Buzz in air", 15);
	else if(statVar.ena_vector&ENA_BUZZER) strncpy(system_menu_list[MENUITEM_SETTINGS_BUZZER], "Buzzer on", 15);
	else strncpy(system_menu_list[MENUITEM_SETTINGS_BUZZER], "Buzzer off", 15);

	
	

	vario_menu.no_items = sizeof(menu2_list) / sizeof(menu2_list[0]);
	vario_menu.menu_name = menu2_name;
	vario_menu.menu_id = menu2_id;

	debug_menu.items_array = menu3_list;
	debug_menu.no_items = sizeof(menu3_list) / sizeof(menu3_list[0]);
	debug_menu.menu_name = menu3_name;
	debug_menu.menu_id = menu3_id;

	settings_menu.items_array = system_menu_list;
	settings_menu.no_items = sizeof(system_menu_list) / sizeof(system_menu_list[0]);
	settings_menu.menu_name = system_menu_name;
	settings_menu.menu_id = system_menu_id;


	gauge_menu.items_array = gauge_menu_list;
	gauge_menu.no_items = sizeof(gauge_menu_list) / sizeof(gauge_menu_list[0]);
	gauge_menu.menu_name = gauge_menu_name;
	gauge_menu.menu_id = gauge_menu_id;
	
	gauges_menu.items_array = gauges_menu_list;
	gauges_menu.no_items = sizeof(gauges_menu_list) / sizeof(gauges_menu_list[0]);
	gauges_menu.menu_name = gauges_menu_name;
	gauges_menu.menu_id = gauges_menu_id;

}

void ClearDisplayText()
{
	display.fillRect(0, 33, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_WHITE);
	display.fillRect(0, 102, GxEPD_WIDTH, 32, GxEPD_BLACK);
}

int ButtonEvaluation(menu * menuPointer)
{
	int selected = menuPointer->selected;
	switch(buttons.getButtonPressed())
	{
		case UP:
		selected -= 1;
		if (selected < 0)
		if(menuPointer->is_detailed)
		selected = menuPointer->no_items/2 + selected;
		else
		selected = menuPointer->no_items + selected;
		break;

		case DOWN:
		selected += 1;
		if(menuPointer->is_detailed)
		selected = selected % (menuPointer->no_items/2);
		else
		selected = selected % menuPointer->no_items;
		break;

		case LEFT:
		// Return from menu
		return LEFT;

		case RIGHT:
		case PRESS:
		menuSelector(menuPointer, selected);
		break;

		default:
		break;
	}
	

	menuPointer->selected = selected;

	return 0;
}

int ReadButtonsAndUpdatePeripherals()
{
	while (!buttons.getFlag())
	{
		routine();
	}
}

void PrepareMenu(menu * menuPointer, bool is_detailed=false)
{
	
	if(is_detailed){
		display.fillScreen(GxEPD_WHITE);
		display.fillRect(0, 90, GxEPD_WIDTH, 58, GxEPD_BLACK);

	}
	else{
		display.fillScreen(GxEPD_WHITE);
		display.fillRect(0, 102, GxEPD_WIDTH, 32, GxEPD_BLACK);
	}

	// Draw line
	display.fillRect(0, 30, GxEPD_WIDTH, 3, GxEPD_BLACK);



}

void MenuEntry(menu *menuPointer){
	unsigned long tstart;
	unsigned long tstop;
	PrepareMenu(menuPointer);
	drawMenu(menuPointer);

	while(1)
	{
		ReadButtonsAndUpdatePeripherals();

		if(ButtonEvaluation(menuPointer) == LEFT){
			lastmenutype = menuPointer->is_detailed;
			break;
		}
		if(menuPointer->is_detailed != lastmenutype){
			//		PrepareMenu(menuPointer);
			//		lastmenutype = menuPointer->is_detailed;
		}
		drawMenu(menuPointer);

	}
}

void drawMenu(menu *menuPointer) {

	if(menuPointer->is_detailed)
	drawMenuDetailed(menuPointer);
	else
	drawMenuSimple(menuPointer);
}

void drawMenuSimple(menu *menuPointer) {


	int temp;

	//PrepareMenu(menuPointer);
	

	//write new text
	display.setTextColor(GxEPD_BLACK);
	display.setCursor(10, 20);
	display.print(menuPointer->menu_name);

	for (int i = 0; i < 5; i++) {

		temp = (menuPointer->selected - 2 + i) % menuPointer->no_items;
		if (temp < 0)
		temp = menuPointer->no_items + temp;
		if (i == 2) {

			display.setTextColor(GxEPD_WHITE);
			display.setCursor(10, 60 + 32 * i);
			display.print(menuPointer->items_array[temp]);
			display.setTextColor(GxEPD_BLACK);
			continue;

		}

		display.setCursor(10, 60 + 32 * i);
		display.print(menuPointer->items_array[temp]);
		// display.print(temp);

	}
	display.display(true);
	menuPointer->selected_old = menuPointer->selected;
	
	//erase text from framebuffer to prepare for next screen
	display.setTextColor(GxEPD_WHITE);
	for (int i = 0; i < 5; i++) {
		display.setCursor(10, 60 + 32 * i);
		temp = (menuPointer->selected - 2 + i) % menuPointer->no_items;
		if (temp < 0)		temp = menuPointer->no_items + temp;
		if (i == 2) {
			display.setTextColor(GxEPD_BLACK);
			display.print(menuPointer->items_array[temp]);
			display.setTextColor(GxEPD_WHITE);
			continue;
		}
		display.print(menuPointer->items_array[temp]);
	}
	display.setCursor(10, 20);
	display.print(menuPointer->menu_name);
	display.setTextColor(GxEPD_BLACK);
}

void drawMenuDetailed(menu *menuPointer) {
	int temp;

	PrepareMenu(menuPointer, true);

	for (int i = 0; i < 6; i++) {

		temp = (menuPointer->selected - 1 + i) % ((menuPointer->no_items) / 2);
		if (temp < 0)
		temp = (menuPointer->no_items) / 2 + temp;
		if (i < 3) {
			if (i == 1 ) {

				display.setTextColor(GxEPD_WHITE);
				display.setCursor(10, 58 + 58 * i);
				display.print(menuPointer->items_array[temp]);
				display.setTextColor(GxEPD_BLACK);

				display.setFont(&FreeMonoBold9pt7b);
				display.setTextColor(GxEPD_WHITE);
				display.setCursor(80, 78 + 58 * (i));
				display.print(menuPointer->items_array[temp + (menuPointer->no_items) / 2]);
				display.setTextColor(GxEPD_BLACK);
				display.setFont(&FreeMonoBold12pt7b);
				continue;

			}

			display.setCursor(10, 58 + 58 * i);
			display.print(menuPointer->items_array[temp]);

			display.setFont(&FreeMonoBold9pt7b);
			display.setCursor(80, 78 + 58 * (i));
			display.print(menuPointer->items_array[temp + (menuPointer->no_items) / 2]);
			// display.print(temp);
			display.setFont(&FreeMonoBold12pt7b);
		}
	}

	display.display(true);

}

void menuSelector(menu *menuPointer, int selected) {
	
	
	//---------Top menu
	if (menuPointer->menu_id == MENUID_TOPMENU){
		
		if (selected == MENUITEM_TOPMENU_UTILITIES){
			MenuEntry(&utilities_menu);
		}
		else if (selected == MENUITEM_TOPMENU_SETINGS){
			MenuEntry(&settings_menu);
		}
		else if (selected == MENUITEM_TOPMENU_DEBUG){
			MenuEntry(&debug_menu);
		}
		else if (selected == MENUITEM_TOPMENU_START_TRACKLOG){
			if(!(statVar.ena_vector & ENA_TRACKLOG)){
				strncpy(listTopMenu[MENUITEM_TOPMENU_START_TRACKLOG], "Stop Trcklog", 15);
				statVar.ena_vector |= ENA_TRACKLOG;
				var_takeofftime = rtc.getEpoch();
				var_takeoffalt = alt_filter;
				maxalt = alt_filter;
				minalt = alt_filter;
				maxrise10s = 0;
				minsink10s = 0;

			}
			else{
				strncpy(listTopMenu[MENUITEM_TOPMENU_START_TRACKLOG], "Start Trcklog", 15);
				statVar.ena_vector &= ~(ENA_TRACKLOG);
				showFlightSummary();
			}

		}
		//mass storage
		else if (selected == MENUITEM_TOPMENU_MASS_STORAGE){
			massStorageEna();

		}
		//power off
		else if (selected == MENUITEM_TOPMENU_POWEROFF) {
			powerOff();


		}
	}
	
	//--------utilities menu---------
	if (menuPointer->menu_id == MENUID_UTILITIES) {
		if (selected == MENUITEM_UTILITIES_CHESS){
			playChessGame();
		}
		if (selected == MENUITEM_UTILITIES_COMPASS){
			compass();
		}
		if (selected == MENUITEM_UTILITIES_LOGGER){
			MenuEntry(&logger_menu);
		}
	}
	
	//--------logger menu---------
	if (menuPointer->menu_id == MENUID_LOGGER) {
		if (selected == MENUITEM_LOGGER_MINUTE){
			logger(0);
		}
		if (selected == MENUITEM_LOGGER_HOUR){
			logger(1);
		}
		if (selected == MENUITEM_LOGGER_DAY){
			logger(2);
		}
	}
	
	//--------Settings menu---------
	if (menuPointer->menu_id == MENUID_SETTINGS) {
		
		if (selected == MENUITEM_SETTINGS_ALTIMETER){
			MenuEntry(&altimemter_menu);
		}

		if (selected == MENUITEM_SETTINGS_VARIO){
			MenuEntry(&vario_menu);
		}
		if (selected == MENUITEM_SETTINGS_GPS_POWER){
			if(statVar.ena_vector&ENA_GPS){
				//gps was low power
				if(statVar.ena_vector&ENA_GPS_LOW_POWER){
					SerialUSB.println("1");
					strncpy(system_menu_list[MENUITEM_SETTINGS_GPS_POWER], "GPS full", 15);
					GPS_full();
				}
				//gps was full power
				else {
					SerialUSB.println("2");
					strncpy(system_menu_list[MENUITEM_SETTINGS_GPS_POWER], "GPS off", 15);
					GPS_off();
					
				}
			}
			else{
				SerialUSB.println("3");
				strncpy(system_menu_list[MENUITEM_SETTINGS_GPS_POWER], "GPS low", 15);
				GPS_low();
			}

		}
		if (selected == MENUITEM_SETTINGS_BUZZER){
			if((statVar.ena_vector&ENA_BUZZER)==0){
				statVar.ena_vector |= ENA_BUZZER;
				statVar.ena_vector |= ENA_BUZZ_WHEN_LOGGING;
				strncpy(system_menu_list[MENUITEM_SETTINGS_BUZZER], "Buzz in flig", 15);
			}
			else if(statVar.ena_vector&ENA_BUZZ_WHEN_LOGGING){
				statVar.ena_vector &= ~ENA_BUZZ_WHEN_LOGGING;
				strncpy(system_menu_list[MENUITEM_SETTINGS_BUZZER], "Buzzer on", 15);
				
			}
			else{
				statVar.ena_vector &= ~ENA_BUZZER;
				strncpy(system_menu_list[MENUITEM_SETTINGS_BUZZER], "Buzzer off", 15);
				
			}
			
		}
		if (selected == MENUITEM_SETTINGS_TIME_ZONE){
			statVar.TimeZone = numpad(statVar.TimeZone);
		}
		if (selected == MENUITEM_SETTINGS_SAVE_EEPROM){
			eepromWrite(0, statVar);
		}
		else if (selected == MENUITEM_SETTINGS_SET_DEFAULT){
			setVariablesDefault();

		}

		if (selected == MENUITEM_SETTINGS_HEATER){
			if(statVar.ena_vector & (ENA_HEATER)){
				strncpy(system_menu_list[MENUITEM_SETTINGS_HEATER], "Heater off", 15);

				digitalWrite(HEAT, 0);
				statVar.ena_vector  = statVar.ena_vector & (~(ENA_HEATER));

			}
			else{
				strncpy(system_menu_list[MENUITEM_SETTINGS_HEATER], "Heater on", 15);
				digitalWrite(HEAT, 1);

				statVar.ena_vector  = statVar.ena_vector | (ENA_HEATER);
			}
			
		}
		if (selected == MENUITEM_SETTINGS_GAUGES){
			MenuEntry(&gauges_menu);
		}
		//accelerometer calibration
		if (selected == MENUITEM_SETTINGS_CALIB_ACCEL){
			calibrateAccelerometer();
		}
		//magnetometer calibration
		if (selected == MENUITEM_SETTINGS_CALIB_MAG){
			calibrateMagnetometer();
		}
	}
	//-------------Altimeter menu

	if (menuPointer->menu_id == MENUID_ALTIMETER) {
		if (selected == MENUITEM_ALTIMETER_SET_ALT){
			setSeaPressureFromAltitude(numpad(alt_baro/100), enviromental_data.pressure);
		}
		else if (selected == MENUITEM_ALTIMETER_SET_SEAP){
			setSeaPressure(numpad(statVar.Psea));
		}
		else if (selected == MENUITEM_ALTIMETER_ALT_FROM_GPS){
			if(fix.valid.location)
			setSeaPressureFromAltitude(fix.altitude(), enviromental_data.pressure);
		}
		else if (selected == MENUITEM_ALTIMETER_ALT_FROM_HGT){
			if(fix.valid.location)
			setSeaPressureFromAltitude(ground_level, enviromental_data.pressure);
		}

	}


	//-----------Vario menu
	if (menuPointer->menu_id == MENUID_VARIO) {
		if (selected == MENUITEM_VARIO_THRESHOLD_UP) {
			statVar.th_rise = numpad(statVar.th_rise);
			if (statVar.th_rise < 0)
			statVar.th_rise *= -1;

		}
		else if (selected == MENUITEM_VARIO_THRESHOLD_DOWN) {
			statVar.th_sink = numpad(statVar.th_sink);
			if (statVar.th_sink > 0)
			statVar.th_sink *= -1;


		}
		else if (selected == MENUITEM_VARIO_EQ_RISE) {
		}
		else if (selected == MENUITEM_VARIO_EQ_SINK) {
		}
		else if (selected == MENUITEM_VARIO_ZVAR){
			statVar.zvariance = numpad(statVar.zvariance);
			kalmanFilter3_configure(statVar.zvariance, statVar.accelvariance, 1.0, alt_baro, 0.0 , 0.0);
		}
		else if (selected == MENUITEM_VARIO_ACCELVAR){
			statVar.accelvariance = numpad(statVar.accelvariance);
			kalmanFilter3_configure(statVar.zvariance, statVar.accelvariance, 1.0, alt_baro, 0.0 , 0.0);
		}
		
		else if (selected == MENUITEM_VARIO_AVERAGING){
			statVar.vario_lowpass_coef = numpad(statVar.vario_lowpass_coef);
			String tmpstring =  String(statVar.vario_lowpass_coef);
			tmpstring.toCharArray(vario_menu.items_array[MENUITEM_VARIO_DATA_AVERAGING], 15);
		}

		// ms5611.putSeaLevel(numpad((int)ms5611.readSeaPressure));
	}

	//----------debug menu
	if (menuPointer->menu_id == MENUID_DEBUG) {
		if (selected == MENUITEM_DEBUG_BMX160){
			debugBMX160();

		}
		else if (selected == MENUITEM_DEBUG_LPS33_SI7021){
			debugBME280();

		}
		else if (selected == MENUITEM_DEBUG_MAX17055){
			debugMAX17055();

		}
		else if (selected == MENUITEM_DEBUG_GPS){
			debugGPS();

		}
		else if (selected == MENUITEM_DEBUG_PRINT_EEPROM){
			eeprom.printAll();
		}
		else if (selected == MENUITEM_DEBUG_32KHZ_TEST){
			debugXOSC32k();

		}
		else if (selected == MENUITEM_DEBUG_PRINT_ACCEL){
			printAccelerometerData();

		}

	}
	//gauges menu
	if (menuPointer->menu_id == MENUID_GAUGES) {
		//BMI160
		if (selected == MENUITEM_GAUGES_VARIO){
			gaugepointer = &statVar.varioGauge;
		}
		if (selected == MENUITEM_GAUGES_VARIO_AVG){
			gaugepointer = &statVar.varioAvgGauge;
		}
		if (selected == MENUITEM_GAUGES_ALTITUDE){
			gaugepointer = &statVar.altitudeGauge;
		}
		if (selected == MENUITEM_GAUGES_AGL){
			gaugepointer = &statVar.AGLGauge;
		}
		if (selected == MENUITEM_GAUGES_SPEED){
			gaugepointer = &statVar.speedGauge;
		}
		if (selected == MENUITEM_GAUGES_HEADING){
			gaugepointer = &statVar.headingGauge;
		}
		if (selected == MENUITEM_GAUGES_WIND_SPEED){
			gaugepointer = &statVar.windGauge;
		}
		if (selected == MENUITEM_GAUGES_WIND_DIR){
			gaugepointer = &statVar.windDirGauge;
		}
		if (selected == MENUITEM_GAUGES_TEMPERATURE){
			gaugepointer = &statVar.tempGauge;
		}
		if (selected == MENUITEM_GAUGES_HUMIDITY){
			gaugepointer = &statVar.humidGauge;
		}
		if (selected == MENUITEM_GAUGES_FLIGHT_TIME){
			gaugepointer = &statVar.flightTimeGauge;
		}
		if (selected == MENUITEM_GAUGES_LIFT_TO_DRAG){
			gaugepointer = &statVar.glideRatioGauge;
		}
		if (selected == MENUITEM_GAUGES_ALT_ABOVE_TAKEOFF){
			gaugepointer = &statVar.altAboveTakeoffGauge;
		}
		
		
		setGaugeMenu(gaugepointer);
		MenuEntry(&gauge_menu);
	}







	//----------gauge menu
	if (menuPointer->menu_id == MENUID_GAUGE) {
		//enable
		if (selected == MENUITEM_GAUGE_ENABLE){
			if ((gaugepointer->settings & GAUGE_ENA) == 0)
			{
				strncpy(gauge_menu_list[MENUITEM_GAUGE_ENABLE], "Enabled", 15);
				gaugepointer->settings  |= GAUGE_ENA;
			}
			else{
				strncpy(gauge_menu_list[MENUITEM_GAUGE_ENABLE], "Disabled", 15);
				gaugepointer->settings &= ~GAUGE_ENA;
			}

		}
		//position
		else if (selected == MENUITEM_GAUGE_POSITION){
			int exitloop = 0;
			while(!exitloop){
				display.fillScreen(GxEPD_WHITE);
				printGauges();
				display.display(1);
				if(buttons.getFlag()){
					switch(buttons.getButtonPressed())
					{
						case UP:
						gaugepointer->offset_Y -=5;
						if (gaugepointer->offset_Y <0)
						{
							gaugepointer->offset_Y =0;
						}
						
						break;

						case DOWN:
						gaugepointer->offset_Y +=5;
						if (gaugepointer->offset_Y >200)
						{
							gaugepointer->offset_Y =200;
						}
						break;

						case LEFT:
						gaugepointer->offset_X -=5;
						if (gaugepointer->offset_X <0)
						{
							gaugepointer->offset_X =0;
						}
						break;

						case RIGHT:
						gaugepointer->offset_X +=5;
						if (gaugepointer->offset_X >200)
						{
							gaugepointer->offset_X =200;
						}
						break;
						case PRESS:
						exitloop = 1;
						break;

						default:
						break;
					}
				}
				
			}

			
		}
		//size
		else if (selected == MENUITEM_GAUGE_SIZE){
			int exitloop = 0;
			while(!exitloop){
				display.fillScreen(GxEPD_WHITE);
				printGauges();
				display.display(1);
				if(buttons.getFlag()){
					switch(buttons.getButtonPressed())
					{
						case UP:
						gaugepointer->size_Y -=5;
						if (gaugepointer->size_Y <0)
						{
							gaugepointer->size_Y =0;
						}
						
						break;

						case DOWN:
						gaugepointer->size_Y +=5;
						if (gaugepointer->size_Y >200)
						{
							gaugepointer->size_Y =200;
						}
						break;

						case LEFT:
						gaugepointer->size_X -=5;
						if (gaugepointer->size_X <0)
						{
							gaugepointer->size_X =0;
						}
						break;

						case RIGHT:
						gaugepointer->size_X +=5;
						if (gaugepointer->size_X >200)
						{
							gaugepointer->size_X =200;
						}
						break;
						case PRESS:
						exitloop = 1;
						break;

						default:
						break;
					}
				}
				
			}

			
		}
		//frame
		else if (selected == MENUITEM_GAUGE_FRAME){
			if (gaugepointer->settings & GAUGE_FRAME)
			{
				strncpy(gauge_menu_list[MENUITEM_GAUGE_FRAME], "Frame off", 15);
				gaugepointer->settings &= ~GAUGE_FRAME;
			}
			else{
				strncpy(gauge_menu_list[MENUITEM_GAUGE_FRAME], "Frame on", 15);
				gaugepointer->settings |= GAUGE_FRAME;
			}

		}
		//decimals
		else if (selected == MENUITEM_GAUGE_DECIMALS){
			int number = numpad((gaugepointer->settings & GAUGE_DIGITS_MASK)>>2);
			if(number > 7)
			number = 7;
			else if(number < 0)
			number = 0;
			gaugepointer->settings &= ~GAUGE_DIGITS_MASK;
			gaugepointer->settings |= 4*number;

		}
		//name
		else if (selected == MENUITEM_GAUGE_NAME){
			keypad(gaugepointer->name_shown, 10);

		}
		
		//units
		else if (selected == MENUITEM_GAUGE_UNITS){
			
			//strncpy(gaugepointer->units, keypad(3), 4);

			keypad(gaugepointer->units, 3);


		}
		//font
		else if (selected == MENUITEM_GAUGE_FONT){
			if ((gaugepointer->settings & GAUGE_FONT_MASK) == GAUGE_FONT_3)  //12 18 24
			{
				strncpy(gauge_menu_list[MENUITEM_GAUGE_FONT], "font size 9", 15);
				gaugepointer->settings &= ~GAUGE_FONT_MASK;
				gaugepointer->settings |= GAUGE_FONT_0;
			}
			else if((gaugepointer->settings & GAUGE_FONT_MASK) == GAUGE_FONT_0){
				strncpy(gauge_menu_list[MENUITEM_GAUGE_FONT], "font size 12", 15);
				gaugepointer->settings &= ~GAUGE_FONT_MASK;
				gaugepointer->settings |= GAUGE_FONT_1;
			}
			else if((gaugepointer->settings & GAUGE_FONT_MASK) == GAUGE_FONT_1){
				strncpy(gauge_menu_list[MENUITEM_GAUGE_FONT], "font size 18", 15);
				gaugepointer->settings &= ~GAUGE_FONT_MASK;
				gaugepointer->settings |= GAUGE_FONT_2;
			}
			else{
				strncpy(gauge_menu_list[MENUITEM_GAUGE_FONT], "font size 24", 15);
				gaugepointer->settings &= ~GAUGE_FONT_MASK;
				gaugepointer->settings |= GAUGE_FONT_3;
			}


		}
		//showing plus sign
		else if (selected == MENUITEM_GAUGE_PLUS_SIGN){
			if (gaugepointer->settings & GAUGE_SHOW_PLUS_SIGN)
			{
				strncpy(gauge_menu_list[MENUITEM_GAUGE_PLUS_SIGN], "not showing +", 15);
				gaugepointer->settings &= ~GAUGE_SHOW_PLUS_SIGN;
			}
			else{
				strncpy(gauge_menu_list[MENUITEM_GAUGE_PLUS_SIGN], "showing + sign", 15);
				gaugepointer->settings |= GAUGE_SHOW_PLUS_SIGN;
			}

		}
		//fixed decimals vs fixed number of digits
		else if (selected == MENUITEM_GAUGE_FIXED_DECIMALS){
			if (gaugepointer->settings & GAUGE_VALIDS)
			{
				strncpy(gauge_menu_list[MENUITEM_GAUGE_FIXED_DECIMALS], "fixed decimal", 15);
				strncpy(gauge_menu_list[MENUITEM_GAUGE_DECIMALS], "no. decimals", 15);
				gaugepointer->settings &= ~GAUGE_VALIDS;
			}
			else{
				strncpy(gauge_menu_list[MENUITEM_GAUGE_FIXED_DECIMALS], "fixed digits", 15);
				strncpy(gauge_menu_list[MENUITEM_GAUGE_DECIMALS], "no. digits", 15);
				gaugepointer->settings |= GAUGE_VALIDS;
			}

		}
		
		else if (selected == MENUITEM_GAUGE_AVERAGING){
			gaugepointer->averaging = numpad(gaugepointer->averaging);

		}
		

	}
	
	menu_init();
	display.setFont(&FreeMonoBold12pt7b);
	PrepareMenu(menuPointer);
}

void setGaugeMenu(Gauge *gaugePointer){
	if (gaugepointer->settings & GAUGE_ENA) strncpy(gauge_menu_list[0], "Enabled", 15);
	else strncpy(gauge_menu_list[0], "Disabled", 15);

	if (gaugepointer->settings & GAUGE_FRAME) strncpy(gauge_menu_list[3], "Frame on", 15);
	else strncpy(gauge_menu_list[3], "Frame off", 15);

	if ((gaugepointer->settings & GAUGE_FONT_MASK) == 0) strncpy(gauge_menu_list[7], "font size 9", 15);
	else if ((gaugepointer->settings & GAUGE_FONT_MASK) == 1) strncpy(gauge_menu_list[7], "font size 12", 15);
	else if ((gaugepointer->settings & GAUGE_FONT_MASK) == 2) strncpy(gauge_menu_list[7], "font size 18", 15);
	else strncpy(gauge_menu_list[7], "font size 24", 15);
	

	if (gaugepointer->settings & GAUGE_SHOW_PLUS_SIGN)	strncpy(gauge_menu_list[8], "showing + sgn", 15);
	else strncpy(gauge_menu_list[8], "not showing +", 15);
	
	if(gaugepointer->settings & GAUGE_VALIDS){strncpy(
		gauge_menu_list[9], "fixed digits", 15);
		strncpy(gauge_menu_list[4], "no. digits", 15);
	}
	else{
		strncpy(gauge_menu_list[9], "fixed decimal", 15);
		strncpy(gauge_menu_list[4], "no. decimals", 15);
	}

	
	menu_init();
}

void debugBMX160(void){
	display.setFont();
	while (!buttons.getFlag())	{
		routine();
		display.fillRect(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_WHITE);
		display.setCursor(50, 0);
		display.print("uncor");
		display.setCursor(140, 0);
		display.print("cor");
		display.setCursor(5, 10);
		display.print("ax");
		display.setCursor(35, 10);
		display.print(ax);
		display.setCursor(5, 20);
		display.print("ay");
		display.setCursor(35, 20);
		display.print(ay);
		display.setCursor(5, 30);
		display.print("az");
		display.setCursor(35, 30);
		display.print(az);
		

		display.setCursor(120, 10);
		display.print(ax_corr);

		display.setCursor(120, 20);
		display.print(ay_corr);

		display.setCursor(120, 30);
		display.print(az_corr);
		
		display.setCursor(5, 40);
		display.print("g");
		display.setCursor(35, 40);
		display.print(   pow(  ((float)ax*ax + (float)ay*ay + (float)az*az)/IMU_BIT_PER_G/IMU_BIT_PER_G , 0.5) , 3   );
		display.setCursor(120, 40);
		display.print(   pow(  ((float)ax_corr*ax_corr + (float)ay_corr*ay_corr + (float)az_corr*az_corr)/IMU_BIT_PER_G/IMU_BIT_PER_G , 0.5) , 3   );
		
		display.setCursor(5, 50);
		display.print("gx");
		display.setCursor(35, 50);
		display.print(gx);
		display.setCursor(120, 50);
		display.print(gx/131.2);
		display.setCursor(5, 60);
		display.print("gy");
		display.setCursor(35, 60);
		display.print(gy);
		display.setCursor(120, 60);
		display.print(gy/131.2);
		display.setCursor(5, 70);
		display.print("gz");
		display.setCursor(35, 70);
		display.print(gz);
		display.setCursor(120, 70);
		display.print(gz/131.2);
		
		display.setCursor(5, 80);
		display.print("mx");
		display.setCursor(35, 80);
		display.print(mx_cor);
		display.setCursor(120, 80);

		display.setCursor(5, 90);
		display.print("my");
		display.setCursor(35, 90);
		display.print(my_cor);
		display.setCursor(120, 90);

		display.setCursor(5, 100);
		display.print("mz");
		display.setCursor(35, 100);
		display.print(mz_cor);
		display.setCursor(120, 100);

		
		
		
		display.setCursor(50, 120);
		display.print("gain");
		display.setCursor(140, 120);
		display.print("offset");
		
		display.setCursor(5, 130);
		display.print("x");
		display.setCursor(35, 130);
		display.print(statVar.gainErrorAccelX, 3);
		display.setCursor(140, 130);
		display.print(statVar.offsetAccelX);
		
		display.setCursor(5, 140);
		display.print("y");
		display.setCursor(35, 140);
		display.print(statVar.gainErrorAccelY, 3);
		display.setCursor(140, 140);
		display.print(statVar.offsetAccelY);
		
		display.setCursor(5, 150);
		display.print("z");
		display.setCursor(35, 150);
		display.print(statVar.gainErrorAccelZ, 3);
		display.setCursor(140, 150);
		display.print(statVar.offsetAccelZ);
		display.setCursor(5, 160);
		display.print("yaw");
		display.setCursor(60, 160);
		display.print(yaw);

		display.setCursor(5, 170);
		display.print("pitch");
		display.setCursor(60, 170);
		display.print(pitch);

		display.setCursor(5, 180);
		display.print("roll");
		display.setCursor(60, 180);
		display.print(roll);


		
		display.display(true);


	}
	display.setFont(&FreeMonoBold12pt7b);
	buttons.getButtonPressed();
}

void debugBME280(void){
	display.setFont(&FreeMonoBold9pt7b);
	while (!buttons.getFlag())
	{
		routine();
		display.fillRect(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_WHITE);
		display.setCursor(5, 30);
		display.print("Psea");
		display.setCursor(70, 30);
		display.print(statVar.Psea);
		
		display.setCursor(5, 45);
		display.print("Press");
		display.setCursor(70, 45);
		display.print(enviromental_data.pressure);
		
		display.setCursor(5, 60);
		display.print("Temp");
		display.setCursor(70, 60);
		display.print(enviromental_data.temperature);
		
		display.setCursor(5, 75);
		display.print("Humid");
		display.setCursor(70, 75);
		display.print(enviromental_data.humidity);
		

		
		
		display.display(true);


	}
	display.setFont(&FreeMonoBold12pt7b);
	buttons.getButtonPressed();
	
}

void debugXOSC32k(void){
	display.setFont(&FreeMonoBold9pt7b);
	clk_test();
	//buzzerFreq(1000);
	pinPeripheral(SRAM_CS, PIO_AC_CLK);
	while (!buttons.getFlag())
	{
		routine();
		display.fillRect(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_WHITE);
		display.setCursor(5, 30);
		display.println("32.768kHz on PA21");
		display.println("         (pin 30)");
		display.println("          SRAM_CS");

		
		display.display(true);


	}
	pinMode(SRAM_CS, OUTPUT);
	buzzerInit();
	display.setFont(&FreeMonoBold12pt7b);
	buttons.getButtonPressed();
	
}

void printAccelerometerData(void){
	display.setFont(&FreeMonoBold9pt7b);
	display.fillRect(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_WHITE);
	display.setCursor(5, 30);
	display.println("acel data on serialUSB");
	display.println("ax,ay,az");

	
	display.display(true);
	while (!buttons.getFlag())
	{
		routine();
		delay(100);
		SerialUSB.print(ax);
		SerialUSB.print(",");
		SerialUSB.print(ay);
		SerialUSB.print(",");
		SerialUSB.println(az);


	}
	pinMode(SRAM_CS, OUTPUT);
	buzzerInit();
	display.setFont(&FreeMonoBold12pt7b);
	buttons.getButtonPressed();
	
}

void debugMAX17055(void){
	display.setFont(&FreeMonoBold9pt7b);
	while (!buttons.getFlag())
	{
		routine();
		display.fillRect(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_WHITE);
		display.setCursor(5, 180);
		display.print("Vcurr");
		display.setCursor(70, 180);
		display.print(max17055.getInstantaneousVoltage(), 3); display.print(" V");
		display.setCursor(5, 195);
		display.print("Vavg");
		display.setCursor(70, 195);
		display.print(max17055.getAverageVoltage(), 3); display.print(" V");

		
		display.setCursor(5, 45);
		display.print("Icur");
		display.setCursor(60, 45);
		display.print(max17055.getInstantaneousCurrent());  display.print(" mA");
		display.setCursor(5, 60);
		display.print("Iavg");
		display.setCursor(60, 60);
		display.print(max17055.getAverageCurrent());  display.print(" mA");

		
		
		display.setCursor(5, 75);
		display.print("SOC");
		display.setCursor(50, 75);
		display.print(max17055.getSOC());  display.print(" %");
		display.setCursor(5, 90);
		display.print("TTE");
		display.setCursor(50, 90);
		display.print(max17055.getTimeToEmpty()); display.print(" hr");
		display.setCursor(5, 105);
		display.print("TTF");
		display.setCursor(50, 105);
		display.print(max17055.getTimeToFull()); display.print(" hr");
		
		display.setCursor(5, 120);
		display.print("desCap");
		display.setCursor(90, 120);
		display.print(max17055.getCapacity(), 0); display.print(" mAh");
		display.setCursor(5, 135);
		display.print("measCap");
		display.setCursor(90, 135);
		display.print(max17055.getReportedCapacity()); display.print(" mAH");
		display.setCursor(5, 150);
		display.print("remCap");
		display.setCursor(90, 150);
		display.print(max17055.getRemainingCapacity()); display.print(" mAH");
		display.setCursor(5, 165);
		display.print("Temp");
		display.setCursor(90, 165);
		display.print(max17055.getTemp()); display.print(" degC");

		
		
		
		display.display(true);
		SerialUSB.println(max17055.getVFSOC());


	}
	display.setFont(&FreeMonoBold12pt7b);
	buttons.getButtonPressed();
	
}


void debugGPS(void){
	while (!buttons.getFlag())
	{	routine();
		display.setFont(&FreeMonoBold9pt7b);
		display.fillRect(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_WHITE);
		
		display.setCursor(5,15);
		display.print("stat:");
		if(fix.valid.location)
		display.print("3d fix");
		else if(fix.valid.time)
		display.print("time fix");
		else
		display.print("no fix");

		display.setCursor(5, 30);
		display.print("lat:");
		display.print(fix.latitude(), 5);
		display.setCursor(5, 45);
		display.print("lon:");
		display.print(fix.longitude(),5);
		display.setCursor(5, 60);
		display.print("spd:");
		display.print(fix.speed_kph(),1);
		display.print(" hdg:");
		display.print(fix.heading(),0);
		display.setCursor(5,75);

		

		display.print("alt:");
		display.print(fix.altitude());
		display.setCursor(5,90);
		
		display.print(fix.dateTime.hours); display.print(":");display.print(fix.dateTime.minutes);display.print(":"); display.print(fix.dateTime.seconds);	display.print(" ");display.print(fix.dateTime.date);display.print("/");display.print(fix.dateTime.month); display.print("/");display.print(fix.dateTime.year);

		int satcount = 0;


		for(int i = 0; i < gps.sat_count; i++){
			if(gps.satellites[i].tracked){
				satcount++;
			}
		}
		
		display.writeFastHLine(0, 185, 199, GxEPD_BLACK);
		display.writeFastVLine(20, 95, 110, GxEPD_BLACK);

		int satdrawn = 0;
		display.setFont();
		for(int i = 0; i < gps.sat_count; i++){
			if(gps.satellites[i].tracked){
				display.writeFillRect( 22+satdrawn*180/satcount, 185-gps.satellites[i].snr, 174/satcount-4, gps.satellites[i].snr, GxEPD_BLACK);
				display.setCursor(22+satdrawn*180/satcount,190);
				display.print(gps.satellites[i].id);
				satdrawn++;
			}
		}
		display.setFont(&FreeMonoBold9pt7b);
		
		
		
		
		/*
		SerialUSB.println(" ");
		for(int i = 0; i < gps.sat_count; i++){
		if(gps.satellites[i].tracked){
		SerialUSB.print(gps.satellites[i].id);
		SerialUSB.print(",");
		SerialUSB.print(gps.satellites[i].snr);
		SerialUSB.print(",");
		}
		}
		SerialUSB.println(" ");
		*/



		display.display(true);


	}
	display.setFont(&FreeMonoBold12pt7b);
	buttons.getButtonPressed();
	
}





void calibrateMagnetometer(){
	
	int no_samples = 100;
	int ISR_counter_old = counter_incremented_every_ISR;
	int enavectorold = statVar.ena_vector;
	int16_t x[no_samples];
	int16_t y[no_samples];
	int16_t z[no_samples];
	float xf[no_samples];
	float yf[no_samples];
	float zf[no_samples];
	float fit_center[3] = {0};
	float fit_radius[3] = {1};
	statVar.ena_vector &= ~ENA_BUZZER;
	int volume_old = statVar.BuzzerVolume;
	statVar.BuzzerVolume = 80;
	
	analogWrite(DAC, 50);
	pinPeripheral(BUZZER_PIN, PIO_TIMER);
	buzzerFreq(300);

	buzzerEna(1);
	int i = 0;
	while (i < no_samples){

		while(ISR_counter_old == counter_incremented_every_ISR);
		if(counter_incremented_every_ISR % 10 ==0){
			
			xf[i] = mx/100.0;
			yf[i] = my/100.0;
			zf[i] = mz/100.0;
			ISR_counter_old = counter_incremented_every_ISR;
			i++;
			
			
			

		}


		
	}

	buzzerEna(0);

	int flag = fit_elipsoid(xf,yf,zf,no_samples, fit_center, fit_radius);

	if (!flag) {
		statVar.gainErrorMagX = fit_radius[0]*100000;
		statVar.gainErrorMagY = fit_radius[1]*100000;
		statVar.gainErrorMagZ = fit_radius[2]*100000;
		
		statVar.offsetMagX = fit_center[0]*100000000;
		statVar.offsetMagY = fit_center[1]*100000000;
		statVar.offsetMagZ = fit_center[2]*100000000;
		
	}
	statVar.ena_vector = enavectorold;
	pinMode(BUZZER_PIN, OUTPUT);
	statVar.BuzzerVolume = volume_old;
	
}

void calibrateAccelerometer(){
	
	int no_samples = 6;
	int averaging = 100;
	int ISR_counter_old = counter_incremented_every_ISR;
	int enavectorold = statVar.ena_vector;
	int16_t x[no_samples];
	int16_t y[no_samples];
	int16_t z[no_samples];
	float xf[no_samples];
	float yf[no_samples];
	float zf[no_samples];
	float fit_center[3] = {0};
	float fit_radius[3] = {1};
	statVar.ena_vector &= ~ENA_BUZZER;
	int volume_old = statVar.BuzzerVolume;
	statVar.BuzzerVolume = 80;
	
	analogWrite(DAC, 50);
	pinPeripheral(BUZZER_PIN, PIO_TIMER);
	buzzerFreq(300);

	
	for (int i = 0; i < no_samples; i++){
		buzzerEna(1);
		long avgx = 0;
		long avgy = 0;
		long avgz = 0;
		
		for(int j = 0; j < averaging; j++){
			while(ISR_counter_old == counter_incremented_every_ISR);
			
			avgx += ax;
			avgy += ay;
			avgz += az;
			ISR_counter_old = counter_incremented_every_ISR;
		}
		x[i] = avgx/averaging;
		y[i] = avgy/averaging;
		z[i] = avgz/averaging;
		for(int i = 0; i < no_samples; i++){
			xf[i] = x[i]/IMU_BIT_PER_G;
			yf[i] = y[i]/IMU_BIT_PER_G;
			zf[i] = z[i]/IMU_BIT_PER_G;/*
			xf[i] = x[i];
			yf[i] = y[i];
			zf[i] = z[i];
			*/
		}
		
		
		buzzerEna(0);
		delay(2000);


		
	}

	

	int flag = fit_elipsoid(xf,yf,zf,no_samples, fit_center, fit_radius);

	if (!flag) {
		statVar.gainErrorAccelX = 1/fit_radius[0];
		statVar.gainErrorAccelY = 1/fit_radius[1];
		statVar.gainErrorAccelZ = 1/fit_radius[2];
		
		statVar.offsetAccelX = fit_center[0]*IMU_BIT_PER_G;
		statVar.offsetAccelY = fit_center[1]*IMU_BIT_PER_G;
		statVar.offsetAccelZ = fit_center[2]*IMU_BIT_PER_G;
		
	}
	statVar.ena_vector = enavectorold;
	pinMode(BUZZER_PIN, OUTPUT);
	statVar.BuzzerVolume = volume_old;
	
}

void showFlightSummary(void){
	
	display.setFont(&FreeMonoBold9pt7b);
	display.fillScreen(GxEPD_WHITE);
	
	display.setCursor(5,15);
	display.print("takeoff: ");
	display.print(var_takeoffalt*0.01f, 0);
	display.print("m");
	
	display.setCursor(5,30);
	display.print("lading:");
	display.print(alt_filter*0.01f, 0);
	display.print("m");
	
	display.setCursor(5,45);
	display.print("max alt:");
	display.print(maxalt*0.01f, 0);
	display.print("m");
	
	display.setCursor(5,60);
	display.print("min alt:");
	display.print(minalt*0.01f, 0);
	display.print("m");
	
	display.setCursor(5,75);
	display.print("max rise:");
	display.print(maxrise10s*0.01f);
	display.print("m");
	
	display.setCursor(5,90);
	display.print("max sink:");
	display.print(minsink10s*0.01f);
	display.print("m");
	
	display.setCursor(5,105);
	display.print("flight time sec:");
	display.print(rtc.getEpoch()-var_takeofftime);
	display.print("m");
	
	
	

	display.display(true);

	
	while (!buttons.getFlag())
	{	routine();
		

	}
	display.setFont(&FreeMonoBold12pt7b);
	buttons.getButtonPressed();
	
}