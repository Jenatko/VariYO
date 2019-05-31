#include <GxGDEP015OC1/GxGDEP015OC1.h>
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

#include "MadgwickAHRS.h"


char listTopMenu[][15] = {"0. Peripher", "1. System", "2. Vario", "3. Debug", "Start Trcklog", "MassStorage1", "Power Off"};
char topMenuName[15] = "TopMenu";
int topMenuID = 0x01;

char menu0_list[][15] = {"0.0 Altimeter", "0.1 Buzzer", "0.2 GPS full", "0.3 GPS low", "0.4 GPS_off"};
char menu0_name[15] = "Periph";
int  menu0_id = 0x10;

char menu1_list[][15] = {"0.0.0 set Alt", "0.0.1 set SeaP", "0.0.2 item"};
char menu1_name[15] = "Altimeter";
int  menu1_id = 0x11;

char menu2_list[][15] = {"2.0 Thres. up", "2.1 Thresh. dn", "Equation rise", "Equation sink", "rise_data", "sink_data", "", ""};
char menu2_name[15] = "Vario";
int  menu2_id = 0x12;

char menu3_list[][15] = {"BMIX60", "LPS33,SI7021", "MAX17055", "GPS", "set Default","print EEPROM", "32kHz test", "Print accel"};
char menu3_name[15] = "3. Debug";
int  menu3_id = 0x13;

char system_menu_list[][15] = {"4.0 Time zone", "4.1 Chess", "4.2 Zvar", "4.3 accelvar", "4.4 saveEEPROM", "heater off"};
char system_menu_name[15] = "System";
int  system_menu_id = 0x14;

int lastmenutype = 0;


menu topmenu;
menu menu0;
menu menu1;
menu menu2;
menu menu3;
menu system_menu;


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
	menu2.is_detailed = true;
	String tmpstring =  String((float)statVar.th_rise / 100);
	tmpstring.concat(" m/s");
	tmpstring.toCharArray(menu2.pole[4], 15);

	tmpstring =  String((float)statVar.th_sink / 100);
	tmpstring.concat(" m/s");
	tmpstring.toCharArray(menu2.pole[5], 15);

	menu2.velikost = sizeof(menu2_list) / sizeof(menu2_list[0]);
	menu2.jmeno_menu = menu2_name;
	menu2.menu_id = menu2_id;

	menu3.pole = menu3_list;
	menu3.velikost = sizeof(menu3_list) / sizeof(menu3_list[0]);
	menu3.jmeno_menu = menu3_name;
	menu3.menu_id = menu3_id;

	system_menu.pole = system_menu_list;
	system_menu.velikost = sizeof(system_menu_list) / sizeof(system_menu_list[0]);
	system_menu.jmeno_menu = system_menu_name;
	system_menu.menu_id = system_menu_id;


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
		selected = menuPointer->velikost + selected;
		break;

		case DOWN:
		selected += 1;
		selected = selected % menuPointer->velikost;
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
		buttons.buttonUpdate();

		// Read GPS
		routine();
	}
}



void PrepareMenu(menu * menuPointer, bool is_detailed=false)
{
	
	if(is_detailed){
		display.fillRect(0, 0, GxEPD_WIDTH, 89, GxEPD_WHITE);
		display.fillRect(0, 90, GxEPD_WIDTH, 58, GxEPD_BLACK);
		display.fillRect(0, 149, GxEPD_WIDTH, 52, GxEPD_WHITE);
	}
	else{
		display.fillRect(0, 0, GxEPD_WIDTH, 102, GxEPD_WHITE);
		display.fillRect(0, 102, GxEPD_WIDTH, 32, GxEPD_BLACK);
		display.fillRect(0, 135, GxEPD_WIDTH, 65, GxEPD_WHITE);
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
		
		tstop = micros();
		//SerialUSB.println((tstop-tstart)/1000);
		ReadButtonsAndUpdatePeripherals();
		tstart = micros();
		
		//ClearDisplayText();

		if(ButtonEvaluation(menuPointer) == LEFT){
			lastmenutype = menuPointer->is_detailed;
			break;
		}
		if(menuPointer->is_detailed != lastmenutype){
			PrepareMenu(menuPointer);
			lastmenutype = menuPointer->is_detailed;
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
	display.print(menuPointer->jmeno_menu);

	for (int i = 0; i < 5; i++) {

		temp = (menuPointer->selected - 2 + i) % menuPointer->velikost;
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
	menuPointer->selected_old = menuPointer->selected;
	
	//erase text from framebuffer to prepare for next screen
	display.setTextColor(GxEPD_WHITE);
	for (int i = 0; i < 5; i++) {
		display.setCursor(10, 60 + 32 * i);
		temp = (menuPointer->selected - 2 + i) % menuPointer->velikost;
		if (temp < 0)		temp = menuPointer->velikost + temp;
		if (i == 2) {
			display.setTextColor(GxEPD_BLACK);
			display.print(menuPointer->pole[temp]);
			display.setTextColor(GxEPD_WHITE);
			continue;
		}
		display.print(menuPointer->pole[temp]);
	}
	display.setCursor(10, 20);
	display.print(menuPointer->jmeno_menu);
	display.setTextColor(GxEPD_BLACK);
}

void drawMenuDetailed(menu *menuPointer) {
	int temp;

	PrepareMenu(menuPointer, true);

	for (int i = 0; i < 6; i++) {

		temp = (menuPointer->selected - 1 + i) % ((menuPointer->velikost) / 2);
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

}

void menuSelector(menu *menuPointer, int selected) {
	
	
	//---------Top menu
	if (menuPointer->menu_id == 0x01){
		//peripher
		if (selected == 0){
			MenuEntry(&menu0);
		}
		//system
		else if (selected == 1){
			MenuEntry(&system_menu);
		}
		//vario
		else if (selected == 2){
			MenuEntry(&menu2);
		}
		//debug
		else if (selected == 3){
			MenuEntry(&menu3);
		}
		//ena/dis tracklog
		else if (selected == 4){
			if(!(statVar.ena_vector & ENA_TRACKLOG)){
				strncpy(listTopMenu[4], "Stop Trcklog", 15);
				menu_init();
				statVar.ena_vector |= ENA_TRACKLOG;

				var_takeofftime = rtc.getEpoch();

			}
			else{
				strncpy(listTopMenu[4], "Start Trcklog", 15);
				menu_init();
				statVar.ena_vector &= ~(ENA_TRACKLOG);
			}
		}
		//mass storage
		else if (selected == 5){
			massStorageEna();
			PrepareMenu(menuPointer);
		}
		//power off
		else if (selected == 6) {
			powerOff();
			buttons.getButtonPressed();
			PrepareMenu(menuPointer);
		}
	}
	
	//--------periph menu---------
	if (menuPointer->menu_id == 0x10) {
		//altimeter
		if (selected == 0){
			MenuEntry(&menu1);
		}
		//GPS modes
		if (selected == 2){
			GPS_full();
			SerialUSB.println("gps full");
		}
		else if (selected == 3){
			GPS_low();
			SerialUSB.println("gps low");
		}
		else if (selected == 4){
			GPS_off();
			SerialUSB.println("gps off");
		}
	}
	
	//--------System menu---------
	if (menuPointer->menu_id == 0x14) {
		//altimeter
		if (selected == 0){
			statVar.TimeZone = numpad(statVar.TimeZone);
			PrepareMenu(menuPointer);
		}
				if (selected == 1){
					playChessGame();
					PrepareMenu(menuPointer);
				}
				if (selected == 2){
					statVar.zvariance = numpad(statVar.zvariance);
					kalmanFilter3_configure(statVar.zvariance, statVar.accelvariance, 1.0, alt_baro, 0.0 , 0.0);
					PrepareMenu(menuPointer);
				}
				if (selected == 3){
					statVar.accelvariance = numpad(statVar.accelvariance);
					kalmanFilter3_configure(statVar.zvariance, statVar.accelvariance, 1.0, alt_baro, 0.0 , 0.0);
					PrepareMenu(menuPointer);
				}
				if (selected == 4){
					eepromWrite(0, statVar);
				}
								if (selected == 5){
							if(statVar.ena_vector & (ENA_HEATER)){
								strncpy(system_menu_list[5], "heater off", 15);
								menu_init();
								digitalWrite(HEAT, 0);
								statVar.ena_vector  = statVar.ena_vector & (~(ENA_HEATER));

							}
							else{
								strncpy(system_menu_list[5], "heater on", 15);
								digitalWrite(HEAT, 1);
								menu_init();
								statVar.ena_vector  = statVar.ena_vector | (ENA_HEATER);
							}
			
								}
				
				
	}
	//-------------Altimeter menu

	if (menuPointer->menu_id == 0x11) {
		if (selected == 0){
			SerialUSB.println(alt_baro);
			setSeaPressureFromAltitude(numpad(alt_baro/100), enviromental_data.pressure);
			SerialUSB.println("setsealevel");
		}
		else if (selected == 1){
			SerialUSB.println("putsealevel");
			setSeaPressure(numpad(statVar.Psea));
			//	ms5611.putSeaLevel(numpad(ms5611.readSeaPressure()));
		}
		// ms5611.putSeaLevel(numpad((int)ms5611.readSeaPressure));
		PrepareMenu(menuPointer);
	}
	

	//-----------Vario menu
	if (menuPointer->menu_id == 0x12) {
		if (selected == 0) {
			statVar.th_rise = numpad(statVar.th_rise);
			if (statVar.th_rise < 0)
			statVar.th_rise *= -1;
			menu_init() ;
		}
		else if (selected == 1) {
			statVar.th_sink = numpad(statVar.th_sink);
			if (statVar.th_sink > 0)
			statVar.th_sink *= -1;

			menu_init() ;

		}
		PrepareMenu(menuPointer);
		// ms5611.putSeaLevel(numpad((int)ms5611.readSeaPressure));
	}
	
	//----------debug menu
	if (menuPointer->menu_id == 0x13) {
		//BMI160
		if (selected == 0){
			debugBMI160();
			PrepareMenu(menuPointer);
		}
		//BME280
		else if (selected == 1){
			debugBME280();
			PrepareMenu(menuPointer);
		}
		//MAX17055
		else if (selected == 2){
			debugMAX17055();
			PrepareMenu(menuPointer);
		}
		//GPS
		else if (selected == 3){
			debugGPS();
			PrepareMenu(menuPointer);
		}
		//default EEPROM
		else if (selected == 4){
			setVariablesDefault();
			menu_init();
		}
		//read EEPROM
		else if (selected == 5){
			eeprom.printAll();
		}
		
		//clk test
		else if (selected == 6){
			debugXOSC32k();
			PrepareMenu(menuPointer);

		}
		//print accelerometer data to serial
				else if (selected == 7){
					printAccelerometerData();
					PrepareMenu(menuPointer);

				}

	}


}

void debugBMI160(void){
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
		display.print(   pow(  ((float)ax*ax + (float)ay*ay + (float)az*az)/268.435456e6 , 0.5) , 3   );
		display.setCursor(120, 40);
		display.print(   pow(  ((float)ax_corr*ax_corr + (float)ay_corr*ay_corr + (float)az_corr*az_corr)/268.435456e6 , 0.5) , 3   );
		
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


		
		display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT);


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
		

		
		
		display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT);


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

		
		display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT);


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

			
			display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT);
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

		
		
		
		display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT);
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



		display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT);


	}
	display.setFont(&FreeMonoBold12pt7b);
	buttons.getButtonPressed();
	
}