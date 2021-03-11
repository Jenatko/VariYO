/*
 * compass.cpp
 *
 * Created: 27/06/2019 17:19:45
 *  Author: tcwrhb
 */ 
#include "button_control.h"
#include "Variables.h"
#include "routine.h"
#include <GxEPD2_BW.h>
//#include <GxGDEP015OC1/GxGDEP015OC1.h>
#include <Fonts/FreeMonoBold24pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>


#include "MadgwickAHRS.h"

#define COMPASS_VERT_CENTRE  110
#define COMPASS_DIAMETER 80

#define UPRIGHT_MODE 1
#define FLAT_MODE 0

void draw_line_polar(int dist_start, int dist_stop, float angle){
	int x1 =100 +  dist_start * sin((angle)*DEG2RAD);
	int y1 = COMPASS_VERT_CENTRE+ dist_start * cos((angle)*DEG2RAD);
		int x2 =100 +  dist_stop * sin((angle)*DEG2RAD);
		int y2 = COMPASS_VERT_CENTRE+ dist_stop * cos((angle)*DEG2RAD);
		display.drawLine(x1, y1, x2, y2, GxEPD_BLACK);
	
}

int check_mode(int mode){
	
	if (mode == UPRIGHT_MODE) {
		if (pitch < 20.0f) {
			return FLAT_MODE;
		}
		return UPRIGHT_MODE;
	} 
	if (pitch > 45.0f) {
		return UPRIGHT_MODE;
	}
	return FLAT_MODE;
}

void flat_mode(){
	display.fillScreen(GxEPD_WHITE);
	display.setCursor(80, 18);
	display.print(yaw, 0);
	
	int topx = 100+ COMPASS_DIAMETER * sin((180+yaw)*DEG2RAD);
	int topy = COMPASS_VERT_CENTRE+ COMPASS_DIAMETER * cos((180+yaw)*DEG2RAD);
	int bottomx = 100+ COMPASS_DIAMETER * sin((yaw)*DEG2RAD);
	int bottomy = COMPASS_VERT_CENTRE+ COMPASS_DIAMETER * cos((yaw)*DEG2RAD);
	int side1x = 100+ 10 * sin((yaw-90)*DEG2RAD);
	int side1y = COMPASS_VERT_CENTRE+ 10 * cos((yaw-90)*DEG2RAD);
	int side2x = 100- 10 * sin((yaw-90)*DEG2RAD);
	int side2y = COMPASS_VERT_CENTRE- 10 * cos((yaw-90)*DEG2RAD);
	display.fillTriangle(100, COMPASS_VERT_CENTRE, topx, topy, side1x, side1y, GxEPD_BLACK);
	display.fillTriangle(100, COMPASS_VERT_CENTRE, topx, topy, side2x, side2y, GxEPD_BLACK);
	display.drawLine(bottomx, bottomy, side1x, side1y, GxEPD_BLACK);
	display.drawLine(bottomx, bottomy, side2x, side2y, GxEPD_BLACK);
	display.drawCircle(100, COMPASS_VERT_CENTRE, COMPASS_DIAMETER, GxEPD_BLACK);
	
	for(int i = 0; i < 360; i+=30){
		draw_line_polar(COMPASS_DIAMETER-15, COMPASS_DIAMETER, i+yaw);
	}
	display.display(1);
}

void upright_mode(){
	display.fillScreen(GxEPD_WHITE);
	display.setCursor(80, 18);
	display.print( Madgwick_filter.getAzimuth() , 0);
	display.display(1);

}

void compass(){
	
	static int compass_mode = UPRIGHT_MODE; 
	
	while(buttons.getFlag() == 0){
		
		routine(); // Call this shit for other stuff to happen in background
		
		compass_mode = check_mode(compass_mode);
		
		if (compass_mode == FLAT_MODE) {
			flat_mode();
		} else {
			upright_mode();
		}	
	}
	buttons.getButtonPressed();
}