/*
 * powerModes.h
 *
 * Created: 6/10/2018 18:35:35
 *  Author: Jena
 */ 


#ifndef POWERMODES_H_
#define POWERMODES_H_

void powerOff(int lowVoltage = 0) ;
void allLow(void);
void reinitializePins(void);
void wakeUp(void);
void massStorageEna(void);
void GPS_full(void);
void GPS_low(void);
void GPS_off(void);
void GPS_stopped(void);

void gpsBckpTimer(void);


#endif /* POWERMODES_H_ */