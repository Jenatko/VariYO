/*
* Gauge.cpp
*
* Created: 20/09/2019 12:59:50
*  Author: tcwrhb
*/

#include "Gauge.h"
#include "definitions.h"
#include <GxEPD2/GxEPD2_BW.h>

#include <Fonts/FreeMonoBold24pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>

void updateGauge(Gauge* gaugepointer, float value){
	for(int i = 0; i<NUM_DESKTOPS; i++){
		if(gaugepointer->settings[i] & GAUGE_ENA){
			if(isnan(value)) gaugepointer->value[i] = NAN;
			else if(isnan(gaugepointer->value[i])) gaugepointer->value[i] = value;
			else{
				if(gaugepointer->averaging[i] == 0)
				gaugepointer->averaging[i] = 1;
				if(gaugepointer->averaging[i] == 1)
				gaugepointer->value[i] = value;
				else
				//	gaugepointer->value[active_desktop] = gaugepointer->value[active_desktop] / gaugepointer->averaging[active_desktop] * (gaugepointer->averaging[active_desktop]-1) + value/gaugepointer->averaging[active_desktop];
				gaugepointer->value[i] = (gaugepointer->value[i]  * (gaugepointer->averaging[i]-1) + value)/gaugepointer->averaging[i];
			}
		}
	}
}

void Gauge_print(Gauge *gau, int inverse_value) {
	digitalWrite(SRAM_CS, 1);
	if (gau->settings[active_desktop] & GAUGE_ENA) {
		
		
		if (gau->settings[active_desktop] & GAUGE_FRAME) {
			display.drawRect(gau->offset_X[active_desktop], gau->offset_Y[active_desktop], gau->size_X[active_desktop], gau->size_Y[active_desktop], GxEPD_BLACK);
			//      display.fillRect(0, 0, 199, 199, GxEPD_BLACK);
			//         display.drawRect(offset_X-1, offset_Y-1, size_X+1, size_Y+1, GxEPD_WHITE);
		}
		if (gau->name_shown[active_desktop][0] != '\0') {
			//	display.fillRect(gau->offset_X[active_desktop] + 5 , gau->offset_Y[active_desktop] - 11, strlen(gau->name_shown[active_desktop]) * 12, 13, GxEPD_WHITE);
			display.setCursor(gau->offset_X[active_desktop] + 8, gau->offset_Y[active_desktop]);
			display.setFont(&FreeMonoBold9pt7b);

			display.print(gau->name_shown[active_desktop]);

			display.setFont(&FreeMonoBold12pt7b);

		}
		if (gau->units[active_desktop][0] != '\0') {
			display.setFont(&FreeMonoBold9pt7b);
			switch (gau->settings[active_desktop] & GAUGE_FONT_MASK) {
				case (0):
				display.setCursor(gau->offset_X[active_desktop] + gau->size_X[active_desktop] - strlen(gau->units[active_desktop]) * 12, gau->offset_Y[active_desktop] + 20);
				break;
				case (1):
				display.setCursor(gau->offset_X[active_desktop] + gau->size_X[active_desktop] - strlen(gau->units[active_desktop]) * 12, gau->offset_Y[active_desktop] + 20);
				break;
				case (2):
				display.setCursor(gau->offset_X[active_desktop] + gau->size_X[active_desktop] - strlen(gau->units[active_desktop]) * 12, gau->offset_Y[active_desktop] + 25);
				break;
				case (3):
				display.setCursor(gau->offset_X[active_desktop] + gau->size_X[active_desktop] - strlen(gau->units[active_desktop]) * 12, gau->offset_Y[active_desktop] + 32);
				break;
			}
			//	display.setCursor(gau->offset_X[active_desktop] + gau->size_X[active_desktop] - strlen(gau->units[active_desktop]) * 12, gau->offset_Y[active_desktop] + gau->size_Y[active_desktop] - 17);
			display.print(gau->units[active_desktop]);
			display.setFont(&FreeMonoBold12pt7b);

		}
		
		
		switch (gau->settings[active_desktop] & GAUGE_FONT_MASK) {
			case (0):
			display.setFont(&FreeMonoBold9pt7b);
			display.setCursor(gau->offset_X[active_desktop] + 5, gau->offset_Y[active_desktop] + 20);
			break;
			case (1):
			display.setFont(&FreeMonoBold12pt7b);
			display.setCursor(gau->offset_X[active_desktop] + 5, gau->offset_Y[active_desktop] + 22);
			break;
			case (2):
			display.setFont(&FreeMonoBold18pt7b);
			display.setCursor(gau->offset_X[active_desktop] + 5, gau->offset_Y[active_desktop] + 28);
			break;
			case (3):
			display.setFont(&FreeMonoBold24pt7b);
			display.setCursor(gau->offset_X[active_desktop] + 5, gau->offset_Y[active_desktop] + 35);
			break;
		}
		//display.fillRect(gau->offset_X[active_desktop] + 1, gau->offset_Y[active_desktop] + 1, gau->size_X[active_desktop] - 2 - strlen(gau->units[active_desktop]) * 12, gau->size_Y[active_desktop] - 2, GxEPD_WHITE);
		//display.setCursor(offset_X + 5, offset_Y + 22);
		digitalWrite(SRAM_CS, 0);
		if(isnan(gau->value[active_desktop])) 			display.print("--");
		else{
			
			if(gau->settings[active_desktop] & GAUGE_SHOW_PLUS_SIGN){
				if(gau->value[active_desktop] >=0)
				display.print("+");
			}
			if(gau->settings[active_desktop] & GAUGE_VALIDS){
				

				
				int digits = (gau->settings[active_desktop] & GAUGE_DIGITS_MASK)>>2;
				int rem_for_decimal = 0;
				int valid_pow = 1;
				
				

				for(int i = 0; i < digits; i++ ) valid_pow *= 10;
				
				if(round(gau->value[active_desktop]) >= valid_pow)
				gau->value[active_desktop] = valid_pow - 1;
				else if(round(gau->value[active_desktop]) <= -valid_pow)
				gau->value[active_desktop] = -valid_pow + 1;
				
				for(int j = valid_pow; j >= 1; j/=10){
					if(abs(gau->value[active_desktop]) >= (j-0.05)){
						if(inverse_value)
						display.print(1.0f/gau->value[active_desktop], rem_for_decimal-1);
						else
						display.print(gau->value[active_desktop], rem_for_decimal-1);
						break;
					}
					if(j == 1){
						if(inverse_value)
						display.print(1.0f/gau->value[active_desktop], rem_for_decimal-1);
						else
						display.print(gau->value[active_desktop], rem_for_decimal-1);
						break;
						
					}
					rem_for_decimal++;
				}
			}
			else{
				if(inverse_value)
				display.print(1.0f/gau->value[active_desktop], (gau->settings[active_desktop] & GAUGE_DIGITS_MASK) >> 2);
				else
				display.print(gau->value[active_desktop], (gau->settings[active_desktop] & GAUGE_DIGITS_MASK) >> 2);
			}
			display.setFont(&FreeMonoBold12pt7b);
		}
	}
}

void Gauge_print_frame(Gauge *gau, int inverse_value) {
	if (gau->settings[active_desktop] & GAUGE_ENA) {
		
		
		if (gau->settings[active_desktop] & GAUGE_FRAME) {
			display.drawRect(gau->offset_X[active_desktop], gau->offset_Y[active_desktop], gau->size_X[active_desktop], gau->size_Y[active_desktop], GxEPD_BLACK);
			//      display.fillRect(0, 0, 199, 199, GxEPD_BLACK);
			//         display.drawRect(offset_X-1, offset_Y-1, size_X+1, size_Y+1, GxEPD_WHITE);
		}
		if (gau->name_shown[active_desktop][0] != '\0') {
			//	display.fillRect(gau->offset_X[active_desktop] + 5 , gau->offset_Y[active_desktop] - 11, strlen(gau->name_shown[active_desktop]) * 12, 13, GxEPD_WHITE);
			display.setFont(&FreeMonoBold9pt7b);
			display.setCursor(gau->offset_X[active_desktop] + 8, gau->offset_Y[active_desktop]);
			display.setFont(&FreeMonoBold9pt7b);

			display.print(gau->name_shown[active_desktop]);

			display.setFont(&FreeMonoBold12pt7b);

		}
		if (gau->units[active_desktop][0] != '\0') {
			display.setFont(&FreeMonoBold9pt7b);
			switch (gau->settings[active_desktop] & GAUGE_FONT_MASK) {
				case (0):
				display.setCursor(gau->offset_X[active_desktop] + gau->size_X[active_desktop] - strlen(gau->units[active_desktop]) * 12, gau->offset_Y[active_desktop] + 20);
				break;
				case (1):
				display.setCursor(gau->offset_X[active_desktop] + gau->size_X[active_desktop] - strlen(gau->units[active_desktop]) * 12, gau->offset_Y[active_desktop] + 20);
				break;
				case (2):
				display.setCursor(gau->offset_X[active_desktop] + gau->size_X[active_desktop] - strlen(gau->units[active_desktop]) * 12, gau->offset_Y[active_desktop] + 25);
				break;
				case (3):
				display.setCursor(gau->offset_X[active_desktop] + gau->size_X[active_desktop] - strlen(gau->units[active_desktop]) * 12, gau->offset_Y[active_desktop] + 32);
				break;
			}
			//	display.setCursor(gau->offset_X[active_desktop] + gau->size_X[active_desktop] - strlen(gau->units[active_desktop]) * 12, gau->offset_Y[active_desktop] + gau->size_Y[active_desktop] - 17);
			display.print(gau->units[active_desktop]);
			display.setFont(&FreeMonoBold12pt7b);

		}
	}
}

void Gauge_print_value(Gauge *gau, int inverse_value) {
	if (gau->settings[active_desktop] & GAUGE_ENA) {
		switch (gau->settings[active_desktop] & GAUGE_FONT_MASK) {
			case (0):
			display.setFont(&FreeMonoBold9pt7b);
			display.setCursor(gau->offset_X[active_desktop] + 5, gau->offset_Y[active_desktop] + 20);
			break;
			case (1):
			display.setFont(&FreeMonoBold12pt7b);
			display.setCursor(gau->offset_X[active_desktop] + 5, gau->offset_Y[active_desktop] + 22);
			break;
			case (2):
			display.setFont(&FreeMonoBold18pt7b);
			display.setCursor(gau->offset_X[active_desktop] + 5, gau->offset_Y[active_desktop] + 28);
			break;
			case (3):
			display.setFont(&FreeMonoBold24pt7b);
			display.setCursor(gau->offset_X[active_desktop] + 5, gau->offset_Y[active_desktop] + 35);
			break;
		}
		
		if(isnan(gau->value[active_desktop])) 			display.print("--");
		else{
			
			if(gau->settings[active_desktop] & GAUGE_SHOW_PLUS_SIGN){
				if(gau->value[active_desktop] >=0)
				display.print("+");
			}
			if(gau->settings[active_desktop] & GAUGE_VALIDS){
				

				
				int digits = (gau->settings[active_desktop] & GAUGE_DIGITS_MASK)>>2;
				int rem_for_decimal = 0;
				int valid_pow = 1;
				
				

				for(int i = 0; i < digits; i++ ) valid_pow *= 10;
				
				if(round(gau->value[active_desktop]) >= valid_pow)
				gau->value[active_desktop] = valid_pow - 1;
				else if(round(gau->value[active_desktop]) <= -valid_pow)
				gau->value[active_desktop] = -valid_pow + 1;
				
				for(int j = valid_pow; j >= 1; j/=10){
					if(abs(gau->value[active_desktop]) >= (j-0.05f)){
						if(inverse_value)
						display.print(1.0f/gau->value[active_desktop], rem_for_decimal-1);
						else
						display.print(gau->value[active_desktop], rem_for_decimal-1);
						break;
					}
					if(j == 1){
						if(inverse_value)
						display.print(1.0f/gau->value[active_desktop], rem_for_decimal-1);
						else
						display.print(gau->value[active_desktop], rem_for_decimal-1);
						break;
						
					}
					rem_for_decimal++;
				}
			}
			else{
				if(inverse_value)
				display.print(1.0f/gau->value[active_desktop], (gau->settings[active_desktop] & GAUGE_DIGITS_MASK) >> 2);
				else
				display.print(gau->value[active_desktop], (gau->settings[active_desktop] & GAUGE_DIGITS_MASK) >> 2);
			}
			display.setFont(&FreeMonoBold12pt7b);
		}
	}
}


void printGauges(){
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
	
	Gauge_print(&statVar.PressureAltGauge);
	Gauge_print(&statVar.MagHdgGauge);
	

}


void printGauges_values(){
	Gauge_print_value(&statVar.varioGauge);
	Gauge_print_value(&statVar.altitudeGauge);
	Gauge_print_value(&statVar.AGLGauge);
	Gauge_print_value(&statVar.tempGauge);
	Gauge_print_value(&statVar.humidGauge);
	
	Gauge_print_value(&statVar.speedGauge);
	Gauge_print_value(&statVar.headingGauge);
	Gauge_print_value(&statVar.windGauge);
	Gauge_print_value(&statVar.windDirGauge);
	Gauge_print_value(&statVar.varioAvgGauge);
	
	Gauge_print_value(&statVar.glideRatioGauge, 1);
	Gauge_print_value(&statVar.flightTimeGauge);
	Gauge_print_value(&statVar.altAboveTakeoffGauge);
	
	Gauge_print_value(&statVar.PressureAltGauge);
	Gauge_print_value(&statVar.MagHdgGauge);
	

}


void printGauges_frames(){
	Gauge_print_frame(&statVar.varioGauge);
	Gauge_print_frame(&statVar.altitudeGauge);
	Gauge_print_frame(&statVar.AGLGauge);
	Gauge_print_frame(&statVar.tempGauge);
	Gauge_print_frame(&statVar.humidGauge);
	
	Gauge_print_frame(&statVar.speedGauge);
	Gauge_print_frame(&statVar.headingGauge);
	Gauge_print_frame(&statVar.windGauge);
	Gauge_print_frame(&statVar.windDirGauge);
	Gauge_print_frame(&statVar.varioAvgGauge);
	
	Gauge_print_frame(&statVar.glideRatioGauge, 1);
	Gauge_print_frame(&statVar.flightTimeGauge);
	Gauge_print_frame(&statVar.altAboveTakeoffGauge);
	
	Gauge_print_frame(&statVar.PressureAltGauge);
	Gauge_print_frame(&statVar.MagHdgGauge);
	

}