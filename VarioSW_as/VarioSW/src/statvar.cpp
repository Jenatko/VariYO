/*
* SD_statvar.cpp
*
* Created: 18.3.2021 21:51:47
*  Author: Jena
*/

#include "SdFat.h"
#include "Variables.h"
#include "Arduino.h"

#include "EEPROM.h"
#include "statvar.h"
#include "FlashStorage/FlashStorage.h"

extern SdFat SD;

SdFile saved_statvar;

#ifndef USEEEPROM
FlashStorage(statVarFlash, StaticVariables);
#endif



void save_statvar(){
	#ifdef USEEEPROM
	eepromWrite(0, statVar);
	#else
	int sdfail = save_statvar_SD(statVar);
	if(sdfail)	statVarFlash.write(statVar);
	#endif
	
}

void load_statvar(){
	#ifdef USEEEPROM
	eepromRead(0, statVar);
	#else
	int sdfail = load_statvar_SD(statVar);
	if(sdfail)	statVar = statVarFlash.read();
	#endif
	
	if(statVar.valid == 0) setVariablesDefault();
	
}


int load_statvar_SD(StaticVariables &param){
	if (present_devices & SD_PRESENT)	{
		if(SD.exists("/statvar.txt")){
			saved_statvar.open("/statvar.txt", O_READ);
		}
	}
	if (saved_statvar.isOpen()){
		byte* p = (byte*)(void*)&param;
		int i;
		for (i = 0; i < sizeof(statVar); i++){
			*p++ = saved_statvar.read();
		}
		
		saved_statvar.close();
		return 0;
	}
	else return 1;
	
	
	
	
}


int save_statvar_SD(StaticVariables &param){
	if (present_devices & SD_PRESENT)	{
		saved_statvar.open("/statvar.txt", O_WRITE | O_CREAT);
		
	}
	if (saved_statvar.isOpen()){
		byte* p = (byte*)(void*)&param;
		int i;
		for (i = 0; i < sizeof(statVar); i++){
			saved_statvar.write(*p++);
		}
		saved_statvar.sync();
		saved_statvar.close();
		return 0;
	}
	else return 1;
	
}


void setVariablesDefault(){
	memset(&statVar, sizeof(statVar), 0);
	statVar = {
		
		.valid = 1,
		.th_rise = 30,
		.th_sink = -150,
		.ena_vector= 0,
		.BuzzerVolume = 0,
		.TimeZone = +1,
		
		//MS5611 baro
		.Psea = 101325,
		
		
		//Accelerometer
		.gainErrorAccelX  = 1,
		.offsetAccelX = 0,
		.gainErrorAccelY = 1,
		.offsetAccelY = 0,
		.gainErrorAccelZ = 1,
		.offsetAccelZ = 0,

		
		//Magnetometer, apply mag_data*1000000
		.gainErrorMagX = 5005,
		.offsetMagX = 23615971,
		.gainErrorMagY = 4821,
		.offsetMagY = -147760,
		.gainErrorMagZ = 4444,
		.offsetMagZ = 21860756,
		
		
		.zvariance = 10000 ,
		.accelvariance = 10
		
		

	};
	
	statVar.varioGauge.settings[active_desktop] = GAUGE_FRAME | GAUGE_ENA | GAUGE_SHOW_PLUS_SIGN | GAUGE_DIGITS_2 | GAUGE_FONT_2 | GAUGE_VALIDS;
	statVar.varioGauge.size_X[active_desktop] = 130;
	statVar.varioGauge.size_Y[active_desktop] = 40;
	statVar.varioGauge.offset_X[active_desktop] = 0;
	statVar.varioGauge.offset_Y[active_desktop] = 30;
	statVar.varioGauge.value[active_desktop] = 1.5;
	String("Vario").toCharArray(statVar.varioGauge.name_shown[active_desktop], 10);
	String("m/s").toCharArray(statVar.varioGauge.units[active_desktop], 4);
	
	statVar.altitudeGauge.settings[active_desktop] = GAUGE_FRAME | GAUGE_ENA | GAUGE_DIGITS_1 | GAUGE_FONT_1;
	statVar.altitudeGauge.size_X[active_desktop] = 120;
	statVar.altitudeGauge.size_Y[active_desktop] = 50;
	statVar.altitudeGauge.offset_X[active_desktop] = 0;
	statVar.altitudeGauge.offset_Y[active_desktop] = 85;
	statVar.altitudeGauge.value[active_desktop] = 1.5;
	String("Altitude").toCharArray(statVar.altitudeGauge.name_shown[active_desktop], 10);
	String("mnm").toCharArray(statVar.altitudeGauge.units[active_desktop], 4);
	

	statVar.AGLGauge.settings[active_desktop] = GAUGE_ENA | GAUGE_DIGITS_0 | GAUGE_FONT_1;
	statVar.AGLGauge.size_X[active_desktop] = 120;
	statVar.AGLGauge.size_Y[active_desktop] = 40;
	statVar.AGLGauge.offset_X[active_desktop] = 0;
	statVar.AGLGauge.offset_Y[active_desktop] = 105;
	statVar.AGLGauge.value[active_desktop] = 1.5;
	
	statVar.tempGauge.settings[active_desktop] = GAUGE_FRAME | GAUGE_ENA | GAUGE_DIGITS_0 | GAUGE_FONT_1;
	statVar.tempGauge.size_X[active_desktop] = 80;
	statVar.tempGauge.size_Y[active_desktop] = 50;
	statVar.tempGauge.offset_X[active_desktop] = 120;
	statVar.tempGauge.offset_Y[active_desktop] = 150;
	statVar.tempGauge.value[active_desktop] = 1.5;
	
	
	statVar.humidGauge.settings[active_desktop] = GAUGE_ENA | GAUGE_DIGITS_0 | GAUGE_FONT_1;
	statVar.humidGauge.size_X[active_desktop] = 60;
	statVar.humidGauge.size_Y[active_desktop] = 20;
	statVar.humidGauge.offset_X[active_desktop] = 120;
	statVar.humidGauge.offset_Y[active_desktop] = 170;
	statVar.humidGauge.value[active_desktop] = 1.5;
	
	statVar.speedGauge.settings[active_desktop] = GAUGE_FRAME | GAUGE_ENA | GAUGE_DIGITS_1 | GAUGE_FONT_1;
	statVar.speedGauge.size_X[active_desktop] = 90;
	statVar.speedGauge.size_Y[active_desktop] = 50;
	statVar.speedGauge.offset_X[active_desktop] = 0;
	statVar.speedGauge.offset_Y[active_desktop] = 150;
	statVar.speedGauge.value[active_desktop] = 1.5;
	
	statVar.headingGauge.settings[active_desktop] = GAUGE_ENA | GAUGE_DIGITS_0 | GAUGE_FONT_1;
	statVar.headingGauge.size_X[active_desktop] = 90;
	statVar.headingGauge.size_Y[active_desktop] = 50;
	statVar.headingGauge.offset_X[active_desktop] = 0;
	statVar.headingGauge.offset_Y[active_desktop] = 170;
	statVar.headingGauge.value[active_desktop] = 1.5;
	
	
	
	String("").toCharArray(statVar.AGLGauge.name_shown[active_desktop], 10);
	String("AGL").toCharArray(statVar.AGLGauge.units[active_desktop], 4);
	

	String("Air").toCharArray(statVar.tempGauge.name_shown[active_desktop], 10);
	String("*C").toCharArray(statVar.tempGauge.units[active_desktop], 4);
	String("").toCharArray(statVar.humidGauge.name_shown[active_desktop], 10);
	String("%").toCharArray(statVar.humidGauge.units[active_desktop], 4);
	
	String("Speed").toCharArray(statVar.speedGauge.name_shown[active_desktop], 10);
	String("kph").toCharArray(statVar.speedGauge.units[active_desktop], 4);
	String("").toCharArray(statVar.headingGauge.name_shown[active_desktop], 10);
	String("deg").toCharArray(statVar.headingGauge.units[active_desktop], 4);
	
	String("").toCharArray(statVar.windGauge.name_shown[active_desktop], 10);
	String("kph").toCharArray(statVar.windGauge.units[active_desktop], 4);
	String("").toCharArray(statVar.windDirGauge.name_shown[active_desktop], 10);
	String("deg").toCharArray(statVar.windDirGauge.units[active_desktop], 4);
	
}

