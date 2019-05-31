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


void routine(void);

void update_tracklog(void);

void update_wind(void);

void alt_agl(void);




#endif /* ROUTINE_H_ */