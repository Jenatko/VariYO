/*
 * buzzer.cpp
 *
 * Created: 6/10/2018 21:49:32
 *  Author: Jena
 */ 

#include "buzzer.h"
#include <Arduino.h>
#include "definitions.h"
#include "wiring_private.h"
#include "Variables.h"

 volatile int buzzerRepeatCounterMax;
 volatile int buzzerEnaMax;
 volatile int buzzerRepeatCounter;

void buzzerInit() {

	REG_GCLK_GENDIV = GCLK_GENDIV_DIV(8) |          // Divide the 48MHz clock source by divisor 8: 48MHz/3=6MHz
	GCLK_GENDIV_ID(5);            // Select Generic Clock (GCLK) 5
	while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

	REG_GCLK_GENCTRL = GCLK_GENCTRL_IDC |           // Set the duty cycle to 50/50 HIGH/LOW
	GCLK_GENCTRL_GENEN |         // Enable GCLK5
	GCLK_GENCTRL_SRC_DFLL48M |   // Set the 48MHz clock source
	GCLK_GENCTRL_ID(5);          // Select GCLK5
	while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

	// Feed GCLK5 to TCC2 and TC3
	REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN |         // Enable GCLK5 to TCC2 and TC3
	GCLK_CLKCTRL_GEN_GCLK5 |     // Select GCLK5
	GCLK_CLKCTRL_ID_TCC2_TC3;     // Feed the GCLK4 to TC4 and TC5
	while (GCLK->STATUS.bit.SYNCBUSY);

	REG_TC3_COUNT16_CC0 = 25000;                      // Set the TC4 CC0 register to some arbitary value
	while (TC3->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization
	REG_TC3_COUNT16_CC1 = 0x00;                      // Set the TC4 CC1 register to some arbitary value
	while (TC3->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization
	REG_TC3_COUNT8_PER = 0x40000;                      // Set the PER (period) register to its maximum value
	while (TC3->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization

	REG_TC3_CTRLA |= TC_CTRLA_MODE_COUNT16;           // Set the counter to 8-bit mode
	while (TC3->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization

	REG_TC3_CTRLA |= TC_CTRLA_PRESCALER_DIV1 |     // Set prescaler to 64, 16MHz/64 = 256kHz
	TC_CTRLA_WAVEGEN_MFRQ |
	TC_CTRLA_ENABLE;               // Enable TC4
	while (TC3->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization

}

void clk_test(){
	REG_GCLK_GENDIV = GCLK_GENDIV_DIV(0) |          // Divide the 48MHz clock source by divisor 8: 48MHz/3=6MHz
	GCLK_GENDIV_ID(5);            // Select Generic Clock (GCLK) 5
	while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

	REG_GCLK_GENCTRL = GCLK_GENCTRL_OE |
	GCLK_GENCTRL_IDC |           // Set the duty cycle to 50/50 HIGH/LOW
	GCLK_GENCTRL_GENEN |         // Enable GCLK5
	GCLK_GENCTRL_SRC_XOSC32K |   // Set the 32khz osc clock source
	GCLK_GENCTRL_ID(5);          // Select GCLK5
	while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization
	
	REG_TC3_COUNT16_CC0 = 00;                      // Set the TC4 CC0 register to some arbitary value
	while (TC3->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization
	REG_TC3_COUNT16_CC1 = 0x00;                      // Set the TC4 CC1 register to some arbitary value
	while (TC3->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization
	REG_TC3_COUNT8_PER = 0x000;                      // Set the PER (period) register to its maximum value
	while (TC3->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization

	REG_TC3_CTRLA |= TC_CTRLA_MODE_COUNT16;           // Set the counter to 8-bit mode
	while (TC3->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization

	REG_TC3_CTRLA |= TC_CTRLA_PRESCALER_DIV1 |     // Set prescaler to 64, 16MHz/64 = 256kHz
	TC_CTRLA_WAVEGEN_MFRQ |
	TC_CTRLA_ENABLE;               // Enable TC4
	while (TC3->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization
	
	
}

void buzzerEna(int enable) {
	if (enable) {
		pinPeripheral(BUZZER_PIN, PIO_TIMER);
	}
	else {
		pinMode(BUZZER_PIN, OUTPUT);
		digitalWrite(BUZZER_PIN, 0);
	}
	
	

}

void buzzerFreq(int freq) {
	REG_TC3_COUNT16_CC0 = (float)BUZZER_TIMER_FREQ / freq;
	while (TC3->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization
}

void buzzerCounter(int count) {
	REG_TC3_COUNT16_CC0 = count;
	while (TC3->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization
}

void buzzerAltitudeDiff(int altDiff_cm_in) {
	int altDiff_cm = altDiff_cm_in;
	if (altDiff_cm > 1000)
	altDiff_cm = 1000;
	if (altDiff_cm < -1000)
	altDiff_cm = -1000;
	if (altDiff_cm < statVar.th_sink) {
		buzzerEnaMax = 0xffff;
		buzzerFreq(altDiff_cm * altDiff_cm * 0.0004 + 0.7222 * altDiff_cm + 450);
	}
	else if (altDiff_cm > statVar.th_rise) {
		buzzerEnaMax = 3;
		buzzerFreq(altDiff_cm * altDiff_cm * 0.0012 + 2.44 * altDiff_cm + 325);
		// buzzerRepeatCounterMax = altDiff_cm * altDiff_cm * 0.00009 - 0.1457 * altDiff_cm + 64.29;
		buzzerRepeatCounterMax = altDiff_cm * altDiff_cm * altDiff_cm * -1.06e-7 + altDiff_cm * altDiff_cm * 2.53e-4 - 0.207 * altDiff_cm + 66;
		//   SerialUSB.println( buzzerRepeatCounterMax);
	}
	else {
		buzzerEnaMax = -1;
		//   SerialUSB.println( "a");
	}
}

void buzzerSetVolume(char* buzzerVolume){
	
	
	}