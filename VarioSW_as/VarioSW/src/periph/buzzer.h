/*
 * buzzer.h
 *
 * Created: 6/10/2018 21:49:48
 *  Author: Jena
 */ 


#ifndef BUZZER_H_
#define BUZZER_H_


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


extern int th_sink;
extern int th_rise;

extern volatile int buzzerRepeatCounterMax;

//length of buzzer on within buzzer beeping period
//no. of interrupt counter periods
extern volatile int buzzerEnaMax;

//length of period of buzzer beeping (on+off time)
//no. of interrupt counter periods
extern volatile int buzzerRepeatCounter;

#endif /* BUZZER_H_ */