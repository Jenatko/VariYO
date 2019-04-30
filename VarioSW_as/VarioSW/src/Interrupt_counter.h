/*
 * Interrupt_counter.h
 *
 * Created: 5/10/2018 22:01:45
 *  Author: Jena
 */ 


#ifndef INTERRUPT_COUNTER_H_
#define INTERRUPT_COUNTER_H_



void counterInit(void);

extern volatile float altChange;

extern float yaw, pitch, roll;
extern float a_vertical_imu;

extern int pocitadlo;



#endif /* INTERRUPT_COUNTER_H_ */