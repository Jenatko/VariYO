/*
* Variables.h
*
* Created: 1.12.2018 16:55:05
*  Author: Jena
*/
#include <Arduino.h>
#include <time.h>
#include <bme280_defs.h>

#include "lib/CircleFit/circle.h"
#include "lib/CircleFit/data.h"


#ifndef VARIABLES_H_
#define VARIABLES_H_





typedef struct Gauge{
	float value;
	uint16_t settings;
	//uint8_t decimals;
	uint8_t size_X;
	uint8_t size_Y;
	uint8_t offset_X;
	uint8_t offset_Y;
	//uint8_t frame;
	//uint8_t font;
	//uint8_t ena;
	//uint8_t showPlusSign;
	char name_shown[10];
	char units[4];
	uint16_t averaging;
} Gauge;



typedef struct StaticVariables{
	int16_t th_rise;
	int16_t th_sink;
	int8_t ena_vector;
	int16_t BuzzerVolume;
	int8_t TimeZone;
	
	//MS5611 baro
	float Psea;
	
	//MAX17055 fuel gauge
	float resistSensor;
	int designCap;
	
	//IMU
	float gainErrorAccelX;
	int offsetAccelX;
	float gainErrorAccelY;
	int offsetAccelY;
	float gainErrorAccelZ;
	int offsetAccelZ;
	
	int gainErrorMagX;
	int offsetMagX;
	int gainErrorMagY;
	int offsetMagY;
	int gainErrorMagZ;
	int offsetMagZ;
	
	int vario_lowpass_coef;
	
	float zvariance, accelvariance;
	Gauge varioGauge;
	Gauge varioAvgGauge;
	Gauge altitudeGauge;
	Gauge AGLGauge;
	Gauge speedGauge;
	Gauge headingGauge;
	Gauge tempGauge;
	Gauge humidGauge;
	Gauge windGauge;
	Gauge windDirGauge;
	Gauge glideRatioGauge;
	Gauge flightTimeGauge;
	Gauge altAboveTakeoffGauge;
	
	
} StaticVariables;

struct enviroData {
	/*! Compensated pressure */
	uint32_t pressure;
	/*! Compensated temperature */
	int32_t temperature;
	/*! Compensated humidity */
	uint32_t humidity;
};




extern double myRealAltitude;
extern volatile float altChange, vario_lowpassed;
extern volatile int counter500ms;
extern int redraw;
extern volatile int ax, ay, az, ax_corr, ay_corr, az_corr, gx, gy, gz, ax_avg, ay_avg, az_avg, gx_avg, gy_avg, gz_avg, mx, my, mz, mx_cor, my_cor, mz_cor;
extern float yaw, pitch, roll;
extern float a_vertical_imu, alt_filter, vario_filter, alt_baro;

extern int tracklog_stat;
//extern struct tm*  var_localtime;
extern struct tm  var_localtime;
extern time_t  var_takeofftime;
extern int var_takeoffalt;
extern float g_meter;
extern int ground_level;
extern int PerformRoutineInWaitWhileBusy;
extern int position_updated;


extern float ground_level_interpol;

extern uint8_t present_devices;

extern float battery_voltage, battery_SOC;

extern int logger_ena;

extern int maxalt ;
extern int minalt ;
extern int maxrise10s;
extern int minsink10s ;





//variables for counting wind
extern float wind_speed_mps;
extern float wind_direction;
extern Data movement_vector_data;
extern Circle movement_circle_fit;


//structure containng pressure, humidity and temperature
extern struct enviroData enviromental_data;



extern StaticVariables statVar;

void setVariablesDefault(void);


#endif /* VARIABLES_H_ */