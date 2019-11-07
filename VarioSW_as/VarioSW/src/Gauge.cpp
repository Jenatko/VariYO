/*
* Gauge.cpp
*
* Created: 20/09/2019 12:59:50
*  Author: tcwrhb
*/

#include "Gauge.h"
#include "definitions.h"
#include <GxEPD2_BW.h>

#include <Fonts/FreeMonoBold24pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>

void updateGauge(Gauge* gaugepointer, float value){
	if(gaugepointer->settings & GAUGE_ENA){
		if(isnan(value)) gaugepointer->value = NAN;
		else if(isnan(gaugepointer->value)) gaugepointer->value = value;
		else{
			if(gaugepointer->averaging == 0)
			gaugepointer->averaging = 1;
			if(gaugepointer->averaging == 1)
			gaugepointer->value = value;
			else
		//	gaugepointer->value = gaugepointer->value / gaugepointer->averaging * (gaugepointer->averaging-1) + value/gaugepointer->averaging;
						gaugepointer->value = (gaugepointer->value  * (gaugepointer->averaging-1) + value)/gaugepointer->averaging;
		}
	}
}


void Gauge_print(Gauge *gau, int inverse_value) {
	if (gau->settings & GAUGE_ENA) {
		
		
		if (gau->settings & GAUGE_FRAME) {
			display.drawRect(gau->offset_X, gau->offset_Y, gau->size_X, gau->size_Y, GxEPD_BLACK);
			//      display.fillRect(0, 0, 199, 199, GxEPD_BLACK);
			//         display.drawRect(offset_X-1, offset_Y-1, size_X+1, size_Y+1, GxEPD_WHITE);
		}
		if (gau->name_shown[0] != '\0') {
			//	display.fillRect(gau->offset_X + 5 , gau->offset_Y - 11, strlen(gau->name_shown) * 12, 13, GxEPD_WHITE);
			display.setCursor(gau->offset_X + 8, gau->offset_Y);
			display.setFont(&FreeMonoBold9pt7b);

			display.print(gau->name_shown);

			display.setFont(&FreeMonoBold12pt7b);

		}
		if (gau->units[0] != '\0') {
			display.setFont(&FreeMonoBold9pt7b);
			switch (gau->settings & GAUGE_FONT_MASK) {
				case (0):
				display.setCursor(gau->offset_X + gau->size_X - strlen(gau->units) * 12, gau->offset_Y + 20);
				break;
				case (1):
				display.setCursor(gau->offset_X + gau->size_X - strlen(gau->units) * 12, gau->offset_Y + 20);
				break;
				case (2):
				display.setCursor(gau->offset_X + gau->size_X - strlen(gau->units) * 12, gau->offset_Y + 25);
				break;
				case (3):
				display.setCursor(gau->offset_X + gau->size_X - strlen(gau->units) * 12, gau->offset_Y + 32);
				break;
			}
			//	display.setCursor(gau->offset_X + gau->size_X - strlen(gau->units) * 12, gau->offset_Y + gau->size_Y - 17);
			display.print(gau->units);
			display.setFont(&FreeMonoBold12pt7b);

		}
		
		
		switch (gau->settings & GAUGE_FONT_MASK) {
			case (0):
			display.setFont(&FreeMonoBold9pt7b);
			display.setCursor(gau->offset_X + 5, gau->offset_Y + 20);
			break;
			case (1):
			display.setFont(&FreeMonoBold12pt7b);
			display.setCursor(gau->offset_X + 5, gau->offset_Y + 22);
			break;
			case (2):
			display.setFont(&FreeMonoBold18pt7b);
			display.setCursor(gau->offset_X + 5, gau->offset_Y + 28);
			break;
			case (3):
			display.setFont(&FreeMonoBold24pt7b);
			display.setCursor(gau->offset_X + 5, gau->offset_Y + 35);
			break;
		}
		//display.fillRect(gau->offset_X + 1, gau->offset_Y + 1, gau->size_X - 2 - strlen(gau->units) * 12, gau->size_Y - 2, GxEPD_WHITE);
		//display.setCursor(offset_X + 5, offset_Y + 22);
		
		if(isnan(gau->value)) 			display.print("--");
		else{
			
			if(gau->settings & GAUGE_SHOW_PLUS_SIGN){
				if(gau->value >=0)
				display.print("+");
			}
			if(gau->settings & GAUGE_VALIDS){
				

				
				int digits = (gau->settings & GAUGE_DIGITS_MASK)>>2;
				int rem_for_decimal = 0;
				int valid_pow = 1;
				
				

				for(int i = 0; i < digits; i++ ) valid_pow *= 10;
				
				if(round(gau->value) >= valid_pow)
				gau->value = valid_pow - 1;
				else if(round(gau->value) <= -valid_pow)
				gau->value = -valid_pow + 1;
				
				for(int j = valid_pow; j >= 1; j/=10){
					if(abs(gau->value) >= (j-0.05)){
						if(inverse_value)
						display.print(1.0f/gau->value, rem_for_decimal-1);
						else
						display.print(gau->value, rem_for_decimal-1);
						break;
					}
					if(j == 1){
						if(inverse_value)
						display.print(1.0f/gau->value, rem_for_decimal-1);
						else
						display.print(gau->value, rem_for_decimal-1);
						break;
						
					}
					rem_for_decimal++;
				}
			}
			else{
				if(inverse_value)
				display.print(1.0f/gau->value, (gau->settings & GAUGE_DIGITS_MASK) >> 2);
				else
				display.print(gau->value, (gau->settings & GAUGE_DIGITS_MASK) >> 2);
			}
			display.setFont(&FreeMonoBold12pt7b);
		}
	}
}


void printGauges(){
	/*
	Gauge_enable(&statVar.varioGauge);
	Gauge_enable(&statVar.altitudeGauge);
	Gauge_enable(&statVar.AGLGauge);
	Gauge_enable(&statVar.tempGauge);
	Gauge_enable(&statVar.humidGauge);
	Gauge_enable(&statVar.speedGauge);
	Gauge_enable(&statVar.headingGauge);
	Gauge_enable(&statVar.windDirGauge);
	Gauge_enable(&statVar.windGauge);
	Gauge_enable(&statVar.varioAvgGauge);
	*/
	Gauge_print(&statVar.varioGauge);
	Gauge_print(&statVar.altitudeGauge);
	Gauge_print(&statVar.AGLGauge);
	Gauge_print(&statVar.tempGauge);
	Gauge_print(&statVar.humidGauge);
	
	Gauge_print(&statVar.speedGauge);
	Gauge_print(&statVar.headingGauge);
	Gauge_print(&statVar.windGauge);
	Gauge_print(&statVar.windDirGauge);
	Gauge_print(&statVar.varioAvgGauge);
	
	Gauge_print(&statVar.glideRatioGauge, 1);
	Gauge_print(&statVar.flightTimeGauge);
	Gauge_print(&statVar.altAboveTakeoffGauge);
	

}