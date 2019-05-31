/*
* Variables.h
*
* Created: 1.12.2018 16:55:05
*  Author: Jena
*/
#include <Arduino.h>
#include <time.h>
#include "bme280_defs.h"

#include "data.h"
#include "circle.h"


#ifndef VARIABLES_H_
#define VARIABLES_H_





typedef struct Gauge{
	float value;
	uint8_t decimals;
	uint8_t size_X;
	uint8_t size_Y;
	uint8_t offset_X;
	uint8_t offset_Y;
	uint8_t frame;
	uint8_t font;
		uint8_t ena;
	char name_shown[10];
	char units[4];
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
		
		float zvariance, accelvariance;
	Gauge varioGauge;
	
	
} StaticVariables;





extern double myRealAltitude;
extern volatile float altChange;
extern int redraw;
extern int ax, ay, az, ax_corr, ay_corr, az_corr, gx, gy, gz, ax_avg, ay_avg, az_avg, gx_avg, gy_avg, gz_avg, mx, my, mz, mx_cor, my_cor, mz_cor;
extern float yaw, pitch, roll;
extern float a_vertical_imu;
extern float alt_filter, vario_filter, alt_baro;
extern int tracklog_stat;
extern struct tm*  var_localtime;
extern time_t  var_takeofftime;
extern float g_meter;
extern int ground_level;

extern uint8_t present_devices;





//variables for counting wind
extern float wind_speed_mps;
extern float wind_direction;
extern Data movement_vector_data;
extern Circle movement_circle_fit;


//structure containng pressure, humidity and temperature
extern struct bme280_data enviromental_data;



extern StaticVariables statVar;

void setVariablesDefault(void);


#endif /* VARIABLES_H_ */