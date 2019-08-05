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



void loggerISR(){

	
}

void logger(){
	
	rtc.setAlarmEpoch(rtc.getEpoch()+(5));  //time of ephemirides validity
	rtc.enableAlarm(rtc.MATCH_SS);

	rtc.attachInterrupt(loggerISR);
	
	logger_ena = 1;
	powerOff(0, 0);
	
}