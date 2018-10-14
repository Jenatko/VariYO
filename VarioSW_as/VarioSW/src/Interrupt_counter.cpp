#include <Arduino.h>
#include "Interrupt_counter.h"
#include "definitions.h"
#include "button_control.h"
#include "MS5611.h"
#include "buzzer.h"
#include "roundbuff.h"



int zoufalepomocnapromenna = 0;
int flag_sec;




void counterInit() { // Set up the generic clock (GCLK4) used to clock timers
	REG_GCLK_GENDIV = GCLK_GENDIV_DIV(3) |          // Divide the 48MHz clock source by divisor 3: 48MHz/3=16MHz
	GCLK_GENDIV_ID(4);            // Select Generic Clock (GCLK) 4
	while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization


	REG_GCLK_GENCTRL = GCLK_GENCTRL_IDC |           // Set the duty cycle to 50/50 HIGH/LOW
	GCLK_GENCTRL_GENEN |         // Enable GCLK4
	GCLK_GENCTRL_SRC_DFLL48M |   // Set the 48MHz clock source
	GCLK_GENCTRL_ID(4);          // Select GCLK4
	while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization


	// Feed GCLK4 to TC4 and TC5
	REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN |         // Enable GCLK4 to TC4 and TC5
	GCLK_CLKCTRL_GEN_GCLK4 |     // Select GCLK4
	GCLK_CLKCTRL_ID_TC4_TC5;     // Feed the GCLK4 to TC4 and TC5
	while (GCLK->STATUS.bit.SYNCBUSY);              // Wait for synchronization

	REG_TC4_CTRLA |= TC_CTRLA_MODE_COUNT8;           // Set the counter to 8-bit mode
	while (TC4->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization

	REG_TC4_COUNT8_CC0 = 0xFF;                      // Set the TC4 CC0 register to some arbitary value
	while (TC4->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization
	REG_TC4_COUNT8_CC1 = 0xFF;                      // Set the TC4 CC1 register to some arbitary value
	while (TC4->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization
	REG_TC4_COUNT8_PER = 0xFF;                      // Set the PER (period) register to its maximum value
	while (TC4->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization









	//NVIC_DisableIRQ(TC4_IRQn);
	//NVIC_ClearPendingIRQ(TC4_IRQn);
	NVIC_SetPriority(TC4_IRQn, 3);    // Set the Nested Vector Interrupt Controller (NVIC) priority for TC4 to 0 (highest)
	NVIC_EnableIRQ(TC4_IRQn);         // Connect TC4 to Nested Vector Interrupt Controller (NVIC)

	REG_TC4_INTFLAG |= TC_INTFLAG_MC1 | TC_INTFLAG_MC0 | TC_INTFLAG_OVF;        // Clear the interrupt flags
	REG_TC4_INTENSET = TC_INTENSET_MC1 | TC_INTENSET_MC0 | TC_INTENSET_OVF;     // Enable TC4 interrupts
	// REG_TC4_INTENCLR = TC_INTENCLR_MC1 | TC_INTENCLR_MC0 | TC_INTENCLR_OVF;     // Disable TC4 interrupts

	REG_TC4_CTRLA |= TC_CTRLA_PRESCALER_DIV1024 |     // Set prescaler to 64, 16MHz/64 = 256kHz
	TC_CTRLA_ENABLE;               // Enable TC4
	while (TC4->COUNT8.STATUS.bit.SYNCBUSY);        // Wait for synchronization





	///////////PIEZO BUZZER////////////////





}


void TC4_Handler()                              // Interrupt Service Routine (ISR) for timer TC4
{
	digitalWrite(SRAM_CS, 0);
	
	//update temperature roughly once a second
	
	if (zoufalepomocnapromenna % 60 == 0) {
		ms5611.tempUpdate(1);
		flag_sec = 1;
	}
	else if ((zoufalepomocnapromenna - 1) % 60 == 0) {
		ms5611.tempUpdate(2);
		ms5611.requestPressure();
	}
	
	
	else {
		//store last pressure
		rbuff.putIn(ms5611.updatePressure());
		ms5611.requestPressure();
		
		float vyska = ms5611.readAltitude(rbuff.getAverage());
		float vyskaLast = ms5611.readAltitude(rbuff.getAverageOld());
		altChange = (vyska - vyskaLast);


		buzzerAltitudeDiff(altChange * 100);

	}

	
	//generate short beep bursts, long beep or be silent
	if (buzzerRepeatCounter < buzzerEnaMax)
	buzzerEna(1);
	else
	buzzerEna(0);
	
	zoufalepomocnapromenna++;
	buzzerRepeatCounter++;
	if (buzzerRepeatCounter > buzzerRepeatCounterMax)
	buzzerRepeatCounter = 0;

	buttons.buttonUpdate();
	
	
	

	digitalWrite(SRAM_CS, 1);
	
	//clear interrupt flags
	REG_TC4_INTFLAG = TC_INTFLAG_MC1;
	REG_TC4_INTFLAG = TC_INTFLAG_OVF;
	REG_TC4_INTFLAG = TC_INTFLAG_MC0;
}

