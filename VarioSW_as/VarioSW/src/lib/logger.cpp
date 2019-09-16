/*
* logger.cpp
*
* Created: 23.7.2019 23:19:43
*  Author: Jena
*/

#include "definitions.h"
#include "RTCZero.h"
#include "powerModes.h"
#include "Variables.h"
#include "button_control.h"
#include "MEMS.h"



void loggerISR(){

	
}

void logger(int freq){
	
	rtc.setAlarmEpoch(rtc.getEpoch()+(5));
	if(freq == 0)
	rtc.enableAlarm(rtc.MATCH_SS);
		if(freq == 1)
		rtc.enableAlarm(rtc.MATCH_MMSS);
			if(freq == 2)
			rtc.enableAlarm(rtc.MATCH_HHMMSS);

	rtc.attachInterrupt(loggerISR);
	
	logger_ena = 1;
	powerOff(0, 0);
	
}