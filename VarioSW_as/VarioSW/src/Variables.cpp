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

int active_desktop = 0;



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


