/*
* logger.cpp
*
* Created: 23.7.2019 23:19:43
*  Author: Jena
*/

#include "definitions.h"
#include "RTCZero.h"
#include "powerModes.h"
#include "SD.h"
#include "Variables.h"
#include "button_control.h"
#include "MEMS.h"

extern File loggerFile;

volatile int loggagain = 1;

void loggerISR(){
	loggagain = 1;
	
}

void logger(){
	
	rtc.setAlarmEpoch(rtc.getEpoch()+(5));  //time of ephemirides validity
	rtc.enableAlarm(rtc.MATCH_SS);

	rtc.attachInterrupt(loggerISR);
	
	//loggerFile = SD.open("logger.csv", FILE_WRITE);
	
	while(!buttons.getFlag()){
		if(loggagain){
			//rtc.setAlarmEpoch(rtc.getEpoch()+(5));
			request_si7021();
			while(!read_si7021());
			loggerFile = SD.open("logger.csv", FILE_WRITE);
			loggerFile.print(rtc.getDay());
			loggerFile.print("/");
			loggerFile.print(rtc.getMonth());
			loggerFile.print("/20");
			
			loggerFile.print(rtc.getYear());
			loggerFile.print(" ");


			loggerFile.print(rtc.getHours());
			loggerFile.print(":");
			loggerFile.print(rtc.getMinutes());
			loggerFile.print(":");
			loggerFile.print(rtc.getSeconds());
			loggerFile.print(",");
			loggerFile.print(enviromental_data.temperature/100.0);
			loggerFile.print(",");
			loggerFile.print(enviromental_data.humidity/100.0);
			loggerFile.print(",");
			loggerFile.println(enviromental_data.pressure);
			loggerFile.close();
			
			loggagain = 0;
			
		}
	}
	
}