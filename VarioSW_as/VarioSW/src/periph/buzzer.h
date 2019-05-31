/*
 * buzzer.h
 *
 * Created: 6/10/2018 21:49:48
 *  Author: Jena
 */ 


#ifndef BUZZER_H_
#define BUZZER_H_

#include <Arduino.h>


//initialize internal counter TC3, fed from GCLK5 for buzzer
void buzzerInit(void);

//set buzzer pin to output (disable) or fed from counter (enable)
void buzzerEna(int enable);

//set buzzer output frequency
void buzzerFreq(int freq);

//set buzzer timer compare register
void buzzerCounter(int count);

//set frequency based on altitude change
void buzzerAltitudeDiff(int altDiff_cm_in);

//to check clk frequency, appears on SRAM_CS pin
void clk_test(void);

//set DAC voltage, set buzzer pin as output if volume = 0
void buzzerSetVolume(uint8_t buzzerVolume);



//period of buzzer (meaning how often in goes beep-beep-beep while climbing, not related to the buzzer frequency) 
//in no. of interrupt cycles
extern volatile int buzzer_period;

//length of buzzer on within buzzer beeping period
//no. of interrupt counter periods
extern volatile int buzzer_on_preiod;

//incrementing in every interrupt, determines whether buzzer is on or off, based on buzzer_perod and buzzer_on_period
extern volatile int buzzer_counter;

#endif /* BUZZER_H_ */