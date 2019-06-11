/*
 * routine.h
 *
 * Created: 16/10/2018 22:39:56
 *  Author: Jena
 */ 


#ifndef ROUTINE_H_
#define ROUTINE_H_

#include <NMEAGPS.h>
#include <GPSport.h>
#include <Streamers.h>

extern NMEAGPS  gps;
extern gps_fix  fix;


void routine(int pollGPS = 0);

void update_tracklog(void);

void update_wind(void);

void alt_agl(void);

void redrawRoutine(void);

void alt_agl_debug(float lat, float lon);


#endif /* ROUTINE_H_ */