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
volatile float altChange;
int ax, ay, az, ax_corr, ay_corr, az_corr, gx, gy, gz, ax_avg, ay_avg, az_avg, gx_avg, gy_avg, gz_avg, mx, my, mz, mx_cor, my_cor, mz_cor;
float yaw, pitch, roll;
float a_vertical_imu;
float alt_filter, vario_filter, alt_baro;
int redraw = 0;
int tracklog_stat = 0;
struct tm* var_localtime;
time_t var_takeofftime;
float g_meter = 1;
int ground_level;


uint8_t present_devices;

struct bme280_data enviromental_data;

float wind_speed_mps;
float wind_direction;

Data movement_vector_data(WIND_RBUFF_SIZE);
Circle movement_circle_fit;





void setVariablesDefault(){
	statVar = {
		.th_rise = 30,
		.th_sink = -150,
		.ena_vector= 0,
		.BuzzerVolume = 128,
		.TimeZone = +1,
		
		//MS5611 baro
		.Psea = 101325,
		
		//MAX17055 fuel gauge
		.resistSensor = 0.039,
		.designCap = 500,
		
		//Accelerometer
		.gainErrorAccelX = 0.97824,
		.offsetAccelX = 87,
		.gainErrorAccelY = 1.00159,
		.offsetAccelY = 686,
		.gainErrorAccelZ  = 1.00203,
		.offsetAccelZ = -431,
		
		//Magnetometer, apply mag_data*1000000
				.gainErrorMagX = 4856,
				.offsetMagX = 37264580,
				.gainErrorMagY = 4844,
				.offsetMagY = 3280550,
				.gainErrorMagZ = 4649,
				.offsetMagZ = 447930,
				
				.zvariance = 40 ,
				 .accelvariance = 10

	};
};