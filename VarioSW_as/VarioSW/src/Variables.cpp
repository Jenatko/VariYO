/*
* Variables.cpp
*
* Created: 1.12.2018 16:55:15
*  Author: Jena
*/
#include <Arduino.h>
#include "Variables.h"
#include "definitions.h"


StaticVariables statVar;



double myRealAltitude= 200;
volatile float altChange, vario_lowpassed_600samples;
volatile int ax, ay, az, ax_corr, ay_corr, az_corr, gx, gy, gz, ax_avg, ay_avg, az_avg, gx_avg, gy_avg, gz_avg, mx, my, mz, mx_cor, my_cor, mz_cor, axmax, aymax, azmax, gxmax, gymax, gzmax, axmin, aymin, azmin, gxmin, gymin, gzmin;
float yaw, pitch, roll;
float a_vertical_imu, alt_filter, vario_filter, alt_baro;
//int redraw = 0;
int tracklog_stat = 0;
//struct tm* var_localtime;
struct tm var_localtime;
time_t var_takeofftime;
float g_meter = 1;
int ground_level, ground_level2;
int PerformRoutineInWaitWhileBusy = 0;
int position_updated = 0;


int var_takeoffalt = 0;
int maxalt = 0;
int minalt = 0;
int maxrise10s = 0;
int minsink10s = 0;
volatile int debugflag = 0;
int serialDebugVector = 0;

float battery_voltage, battery_SOC;


uint8_t present_devices;

struct enviroData enviromental_data;

float wind_speed_mps;
float wind_direction;

Data movement_vector_data(WIND_RBUFF_SIZE);
Circle movement_circle_fit;


float ground_level_interpol;

int logger_ena;

//for debugging, can be moved to their proper scope after
int x1y1, x2y1, x1y2, x2y2, alt11, alt12, alt21, alt22;	
float fractx, fracty, x1inter, x2inter, interpolation;
float lat_dr, lon_dr;


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
		.gainErrorAccelX  = (double)16384/16315,
		.offsetAccelX = 12,
		.gainErrorAccelY = (double)16384/16453,
		.offsetAccelY = 910,
		.gainErrorAccelZ = (double)16384/16639,
		.offsetAccelZ = 328,

		
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
	
	statVar.varioGauge.settings = GAUGE_FRAME | GAUGE_ENA | GAUGE_SHOW_PLUS_SIGN | GAUGE_DIGITS_2 | GAUGE_FONT_2 | GAUGE_VALIDS;
	statVar.varioGauge.size_X = 130;
	statVar.varioGauge.size_Y = 40;
	statVar.varioGauge.offset_X = 0;
	statVar.varioGauge.offset_Y = 30;
	statVar.varioGauge.value = 1.5;
	String("Vario").toCharArray(statVar.varioGauge.name_shown, 10);
	String("m/s").toCharArray(statVar.varioGauge.units, 4);
	
	statVar.altitudeGauge.settings = GAUGE_FRAME | GAUGE_ENA | GAUGE_DIGITS_1 | GAUGE_FONT_1;
	statVar.altitudeGauge.size_X = 120;
	statVar.altitudeGauge.size_Y = 50;
	statVar.altitudeGauge.offset_X = 0;
	statVar.altitudeGauge.offset_Y = 85;
	statVar.altitudeGauge.value = 1.5;
	String("Altitude").toCharArray(statVar.altitudeGauge.name_shown, 10);
	String("mnm").toCharArray(statVar.altitudeGauge.units, 4);
	

	statVar.AGLGauge.settings = GAUGE_ENA | GAUGE_DIGITS_0 | GAUGE_FONT_1;
	statVar.AGLGauge.size_X = 120;
	statVar.AGLGauge.size_Y = 40;
	statVar.AGLGauge.offset_X = 0;
	statVar.AGLGauge.offset_Y = 105;
	statVar.AGLGauge.value = 1.5;
	
	statVar.tempGauge.settings = GAUGE_FRAME | GAUGE_ENA | GAUGE_DIGITS_0 | GAUGE_FONT_1;	
	statVar.tempGauge.size_X = 80;
	statVar.tempGauge.size_Y = 50;
	statVar.tempGauge.offset_X = 120;
	statVar.tempGauge.offset_Y = 150;
	statVar.tempGauge.value = 1.5;
	
	
	statVar.humidGauge.settings = GAUGE_ENA | GAUGE_DIGITS_0 | GAUGE_FONT_1;
	statVar.humidGauge.size_X = 60;
	statVar.humidGauge.size_Y = 20;
	statVar.humidGauge.offset_X = 120;
	statVar.humidGauge.offset_Y = 170;
	statVar.humidGauge.value = 1.5;
	
	statVar.speedGauge.settings = GAUGE_FRAME | GAUGE_ENA | GAUGE_DIGITS_1 | GAUGE_FONT_1;
	statVar.speedGauge.size_X = 90;
	statVar.speedGauge.size_Y = 50;
	statVar.speedGauge.offset_X = 0;
	statVar.speedGauge.offset_Y = 150;
	statVar.speedGauge.value = 1.5;
	
	statVar.headingGauge.settings = GAUGE_ENA | GAUGE_DIGITS_0 | GAUGE_FONT_1;	
	statVar.headingGauge.size_X = 90;
	statVar.headingGauge.size_Y = 50;
	statVar.headingGauge.offset_X = 0;
	statVar.headingGauge.offset_Y = 170;
	statVar.headingGauge.value = 1.5;
	
	
	
	String("").toCharArray(statVar.AGLGauge.name_shown, 10);
	String("AGL").toCharArray(statVar.AGLGauge.units, 4);
	

	String("Air").toCharArray(statVar.tempGauge.name_shown, 10);
	String("*C").toCharArray(statVar.tempGauge.units, 4);
	String("").toCharArray(statVar.humidGauge.name_shown, 10);
	String("%").toCharArray(statVar.humidGauge.units, 4);
	
	String("Speed").toCharArray(statVar.speedGauge.name_shown, 10);
	String("kph").toCharArray(statVar.speedGauge.units, 4);
	String("").toCharArray(statVar.headingGauge.name_shown, 10);
	String("deg").toCharArray(statVar.headingGauge.units, 4);
	
	String("").toCharArray(statVar.windGauge.name_shown, 10);
	String("kph").toCharArray(statVar.windGauge.units, 4);
	String("").toCharArray(statVar.windDirGauge.name_shown, 10);
	String("deg").toCharArray(statVar.windDirGauge.units, 4);
	
}