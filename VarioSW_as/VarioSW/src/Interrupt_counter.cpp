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
#include <MadgwickAHRS.h>


Mahony Mahony_filter;
Madgwick Madgwick_filter;




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

	//read_mag();
	//IMU_read();
	SPI_IRQ.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE0));
	if(present_devices & BMX160_PRESENT){
		for(int i = 0; i < IMU_FifoBytesToRead(); i+=FIFO_FRAME_SIZE){
			
			//		SerialUSB.print(inFifoWaiting);
			//	SerialUSB.print(",");
			//	SerialUSB.println(i);
			
			IMU_ReadFrameFromFifo();
			//	digitalWrite(SRAM_CS, 1);

			ax_corr = (float)(ax-statVar.offsetAccelX) * statVar.gainErrorAccelX ;
			ay_corr = (float)(ay-statVar.offsetAccelY) * statVar.gainErrorAccelY;
			az_corr = (float)(az-statVar.offsetAccelZ) * statVar.gainErrorAccelZ;

			
			mx_cor = mx*1000000 - statVar.offsetMagX;
			my_cor = my*1000000 - statVar.offsetMagY;
			mz_cor = mz*1000000 - statVar.offsetMagZ;
			mx_cor /= statVar.gainErrorMagX;
			my_cor /= statVar.gainErrorMagY;
			mz_cor /= statVar.gainErrorMagZ;
			
			
			
			ax_avg = ((ax_avg<<5)-ax_avg+ax)>>5;
			ay_avg = ((ay_avg<<5)- ay_avg+ay)>>5;
			az_avg = ((az_avg<<5)- az_avg+az)>>5;
			gx_avg = ((gx_avg<<5)- gx_avg+gx)>>5;
			gy_avg = ((gy_avg<<5)- gy_avg +gy)>>5;
			gz_avg = ((gz_avg<<5)- gz_avg+gz)>>5;
			
			//Mahony_filter.updateIMU(gx/131.2f, gy/131.2f, gz/131.2f, ax_corr/16384.0f, ay_corr/16384.0f, az_corr/16384.0f);
			//	digitalWrite(SRAM_CS, 0);
			Madgwick_filter.MadgwickAHRSupdate(gx*(1/131.2f*0.0174533f), gy*(1/131.2f*0.0174533f), gz*(1/131.2f*0.0174533f), ax_corr/*/16384.0f*/, ay_corr/*/16384.0f*/, az_corr/*/16384.0f*/, (float)mx_cor, (float)my_cor, (float)mz_cor);
			//	digitalWrite(SRAM_CS, 1);

		}

		
		//yaw = Mahony_filter.getYaw();
		//pitch = Mahony_filter.getPitch();
		//roll = Mahony_filter.getRoll();


		
		//	digitalWrite(SRAM_CS, 1);
		
		//a_vertical_imu = az/16384.0*cos(roll*0.01745329)*cos(pitch*0.01745329) - ax/16384.0*sin(pitch*0.01745329) + ay/16384.0*sin(roll*0.01745329)*cos(pitch*0.01745329);
		
		a_vertical_imu = Madgwick_filter.getVertical(ax_corr/16384.0f, ay_corr/16384.0f, az_corr/16384.0f);
		
	}

	if(present_devices & LPS33_PRESENT){
		//digitalWrite(SRAM_CS, 0);
		baro_readPressure();
		SPI_IRQ.endTransaction();
		//digitalWrite(SRAM_CS, 1);
		
		
		alt_baro = getAltitude()*100;

		
		//SerialUSB.println(alt_baro);
		kalmanFilter3_update(alt_baro, a_vertical_imu*1000.0f-1000.0f, (float)1/60.0f, &alt_filter, &vario_filter);
		vario_lowpassed = (vario_lowpassed * (statVar.vario_lowpass_coef-1)+ vario_filter)/statVar.vario_lowpass_coef;
	}
	
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
	if(statVar.ena_vector & ENA_BUZZER){
				buzzerAltitudeDiff((int)vario_filter);
		if (buzzer_counter < buzzer_on_preiod)
		buzzerEna(1);
		else
		buzzerEna(0);
	}
	

	
	zoufalepomocnapromenna++;
	buzzer_counter++;
	if (buzzer_counter > buzzer_period)
	buzzer_counter = 0;

	buttons.buttonUpdate();
	
	
	counter_incremented_every_ISR++;
	counter500ms++;

	if(counter_incremented_every_ISR%20 == 0){
		//Mag_print_angles();
		//	print_mag();
	}


	
	//clear interrupt flags
	REG_TC4_INTFLAG = TC_INTFLAG_MC1;
	REG_TC4_INTFLAG = TC_INTFLAG_OVF;
	REG_TC4_INTFLAG = TC_INTFLAG_MC0;
}

