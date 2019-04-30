#include <Arduino.h>
#include "Interrupt_counter.h"
#include "definitions.h"
#include "button_control.h"
#include "MS5611.h"
#include "buzzer.h"
#include "roundbuff.h"
#include <BMI160Gen.h>
#include "kalmanfilter3.h"
#include "Variables.h"
#include "MEMS.h"


#include <MahonyAHRS.h>

Mahony filter;



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
	
//digitalWrite(SRAM_CS, 0);
	
	uint8_t inFifoWaiting = IMU_FifoBytesToRead();
	while (inFifoWaiting != 0) {
		for(int i = 0; i < inFifoWaiting; i+=12){
			
		//	SerialUSB.print(inFifoWaiting);
		//	SerialUSB.print(",");
		//	SerialUSB.println(i);
			
			IMU_ReadFrameFromFifo();
			
			ax_corr = (float)(ax-statVar.offsetX) / statVar.gainErrorX ;
			ay_corr = (float)(ay-statVar.offsetY) / statVar.gainErrorY;
			az_corr = (float)(az-statVar.offsetZ) / statVar.gainErrorZ;
			
			ax_avg = ax_avg*9.0/10.0+ax/10.0;
			ay_avg = ay_avg*9.0/10.0+ay/10.0;
			az_avg = az_avg*9.0/10.0+az/10.0;
			gx_avg = gx_avg*9.0/10.0+gx/10.0;
			gy_avg = gy_avg*9.0/10.0+gy/10.0;
			gz_avg = gz_avg*9.0/10.0+gz/10.0;

			/*	SerialUSB.print(gx);
			SerialUSB.print(" ");
			SerialUSB.print(gy);
			SerialUSB.print(" ");
			SerialUSB.println(gz);
			*/
			
			filter.updateIMU(gx/131.2, gy/131.2, gz/131.2, ax_corr/16384.0, ay_corr/16384.0, az_corr/16384.0);
			

		}
		
		//		if(zoufalepomocnapromenna % 2== 0)
		//		SerialUSB.println(a_vertical_imu, 3);
		inFifoWaiting = IMU_FifoBytesToRead();
	}

	
	yaw = filter.getYaw();
	pitch = filter.getPitch();
	roll = filter.getRoll();

	
	//a_vertical_imu = az/16384.0*cos(roll*0.01745329)*cos(pitch*0.01745329) - ax/16384.0*sin(pitch*0.01745329) + ay/16384.0*sin(roll*0.01745329)*cos(pitch*0.01745329);
	
	a_vertical_imu = filter.getVertical(ax_corr/16384.0, ay_corr/16384.0, az_corr/16384.0);
	
		

	BME_read();



//recalculateTaylor();

digitalWrite(SRAM_CS, 0);	
	float alt = getAltitude()*100;
	digitalWrite(SRAM_CS, 1);	
		
			
	kalmanFilter3_update(alt, a_vertical_imu*1000.0-1000.0, (float)1/60.0, &alt_filter, &vario_filter);



	buzzerAltitudeDiff((int)vario_filter);
	
	/*
	SerialUSB.print(flag_sec++);
	SerialUSB.print(" ");
	SerialUSB.print(pressure_read);
	SerialUSB.print(" ");
	SerialUSB.print(altChange);
	SerialUSB.print(" ");
	SerialUSB.print((int)(a_vertical_imu*1000.0-1000.0));
	SerialUSB.print(" ");
	SerialUSB.print((int)(vyska*100));
	SerialUSB.print(" ");
	SerialUSB.println((int)alt_filter);
	*/
	


	
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
	
	
pocitadlo++;


	
	//clear interrupt flags
	REG_TC4_INTFLAG = TC_INTFLAG_MC1;
	REG_TC4_INTFLAG = TC_INTFLAG_OVF;
	REG_TC4_INTFLAG = TC_INTFLAG_MC0;
}

