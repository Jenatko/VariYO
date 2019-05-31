#include <Arduino.h>
#include <GxGDEP015OC1/GxGDEP015OC1.h>
#include "powerModes.h"
#include "definitions.h"
#include "button_control.h"
#include "Interrupt_counter.h"
#include "wiring_private.h"
#include "EEPROM.h"
#include "Variables.h"
#include "SD.h"
#include "MEMS.h"
#include "RTCZero.h"

extern File tracklog;

void powerOff() {
	if(tracklog_stat == 1){
		tracklog.println("    </trkseg>");
		tracklog.println("  </trk>");
		tracklog.println("</gpx>");
		tracklog.close();
		tracklog_stat = 0;
		statVar.ena_vector &= ~(ENA_TRACKLOG);
	}
	
	
	eepromWrite(0, statVar);
	display.fillRect(0, 0, 199, 199, GxEPD_WHITE);
	display.setCursor(20, 100);
	display.print("Power Off");
	display.update();
	USB->DEVICE.CTRLA.reg &= ~USB_CTRLA_ENABLE;
	//USB->DEVICE.CTRLA.reg &= ~USB_CTRLA_RUNSTDBY;
	SCB->SCR |= 1 << 2;
	EIC->WAKEUP.reg = EIC_WAKEUP_WAKEUPEN9;
	//display.powerDown();
	REG_TC4_INTENCLR = TC_INTENCLR_MC1 | TC_INTENCLR_MC0 | TC_INTENCLR_OVF;
	attachInterrupt(BUTTON_CENTER, wakeUp, LOW);
	// pinMode(BUTTON_LEFT, OUTPUT);
	// pinMode(BUTTON_RIGHT, OUTPUT);
	// pinMode(BUTTON_UP, OUTPUT);
	// pinMode(BUTTON_DOWN, OUTPUT);
	// pinMode(BUTTON_CENTER, INPUT_PULLUP);
	/*USB->DEVICE.CTRLA.reg &= ~USB_CTRLA_ENABLE;
	USB->HOST.CTRLA.bit.ENABLE=0;
	USB->DEVICE.CTRLA.bit.ENABLE=0;
	I2S->CTRLA.bit.ENABLE=0;

	ADC->CTRLA.bit.ENABLE=0;

	DAC->CTRLA.bit.ENABLE=0;*/
	allLow();
	gpsBckpTimer();
	__WFI();
if(digitalRead(BUTTON_CENTER))   //go back to sleep if woken up by RTC timer (gps backup power turning off)
__WFI();
	
	rtc.detachInterrupt();

	reinitializePins();
	counterInit();

	
	
		pinMode(SD_DETECT, INPUT_PULLUP);
		delay(10);
		if(digitalRead(SD_DETECT) == 0){
			present_devices |= SD_PRESENT;
		}
		else{
			present_devices &= ~SD_PRESENT;
		}
		pinMode(SD_DETECT, INPUT);

	
	
	display.init(0);


	display.setTextColor(GxEPD_BLACK);

	display.fillRect(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_WHITE);

	display.update();
	display.setRotation(0);

	display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, false);
	display.setTextWrap(0);
	if(present_devices | BMX160_PRESENT)
		IMU_init();
	if(present_devices | LPS33_PRESENT)		
				lps33_init();
	if(present_devices | SI7021_PRESENT)					
				request_si7021();
	

}


void allLow() {
	digitalWrite(SD_RST, 0);
	
	digitalWrite(GPS_CS, 0);
	digitalWrite(EEPROM_CS, 0);
	digitalWrite(SRAM_CS, 0);
	digitalWrite(BARO_CS, 0);
	digitalWrite(IMU_CS, 0);
	digitalWrite(SD_CS, 0);
	digitalWrite(DISP_RST, 0);
	digitalWrite(DISP_DC,0);
	digitalWrite(DISP_CS, 0);
	pinMode(MISO_PROG, OUTPUT);
	pinMode(MOSI_PROG, OUTPUT);
	pinMode(SCK_PROG, OUTPUT);
	digitalWrite(MISO_PROG, 0);
	digitalWrite(MOSI_PROG, 0);
	digitalWrite(SCK_PROG, 0);
	
	pinMode(MISO_IRQ, OUTPUT);
	pinMode(MOSI_IRQ, OUTPUT);
	pinMode(SCK_IRQ, OUTPUT);
	digitalWrite(MISO_IRQ, 0);
	digitalWrite(MOSI_IRQ, 0);
	digitalWrite(SCK_IRQ, 0);
	digitalWrite(POWER_ENA, 1);
	digitalWrite(HEAT, 0);

	
}


void reinitializePins() {
	
	digitalWrite(GPS_BCKP, 1);
	digitalWrite(POWER_ENA, 0);
	delay(50);
	digitalWrite(EEPROM_CS, 1);
	digitalWrite(SRAM_CS, 1);
	digitalWrite(SD_CS, 1);
	digitalWrite(SD_RST, 0);
	digitalWrite(IMU_CS, 1);
	digitalWrite(GPS_CS, 1);
	digitalWrite(DISP_RST, 1);
	digitalWrite(DISP_DC,1);
	digitalWrite(DISP_CS, 1);
	
		digitalWrite(HEAT, statVar.ena_vector & ENA_HEATER);

	pinPeripheral(MOSI_IRQ, PIO_SERCOM);
	pinPeripheral(MISO_IRQ, PIO_SERCOM);
	pinPeripheral(SCK_IRQ, PIO_SERCOM);

	pinPeripheral(MOSI_PROG, PIO_SERCOM);
	pinPeripheral(MISO_PROG, PIO_SERCOM);
	pinPeripheral(SCK_PROG, PIO_SERCOM);
	

}



void wakeUp() {
	


	detachInterrupt(BUTTON_CENTER);
	REG_EIC_INTENCLR = EIC_INTENCLR_EXTINT9;
	REG_EIC_INTFLAG = EIC_INTFLAG_EXTINT9;


}

void massStorageEna() {
	display.fillRect(0, 0, 199, 199, GxEPD_WHITE);
	display.setCursor(20, 100);
	display.print("USB mass storage");
	display.setCursor(20, 120);
	display.print("push to end");
	display.update();
	display.powerDown();
	
	pinMode(22, INPUT);
	pinMode(23, INPUT);
	pinMode(24, INPUT);
	pinMode(SD_CS, INPUT);
	digitalWrite(SD_RST, 1);
	while(1)
	if (buttons.getFlag()) {
		if (buttons.getButtonPressed() == PRESS)
		break;
	}
	digitalWrite(SD_RST, 0);
	delay(100);
	pinPeripheral(22, PIO_SERCOM);
	pinPeripheral(23, PIO_SERCOM);
	pinPeripheral(24, PIO_SERCOM);
	pinMode(SD_CS, OUTPUT);
	digitalWrite(SD_CS, 1);


	
	display.init(0);

	//	display.update();
}



void GPS_low(void){
	
	statVar.ena_vector |= (ENA_GPS);
	statVar.ena_vector |= (ENA_GPS_LOW_POWER);
	byte m1[] = {0xB5, 0x62, 0x06, 0x86, 0x08, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x97, 0x6F};
	byte m2[] = {0xB5, 0x62, 0x06, 0x86, 0x00, 0x00, 0x8C, 0xAA};

	SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
	digitalWrite(GPS_CS, 0);
	for(int i = 0; i<16; i++){
		SPI.transfer(m1[i]);
	}
	digitalWrite(GPS_CS, 1);

	digitalWrite(GPS_CS, 0);
	for(int i = 0; i<8; i++){
		SPI.transfer(m2[i]);
	}
	digitalWrite(GPS_CS, 1);
}

void GPS_full(void){
	
	statVar.ena_vector |= (ENA_GPS);
	statVar.ena_vector &= ~(ENA_GPS_LOW_POWER);
	
	byte m1[] = {0xB5, 0x62, 0x06, 0x86, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x94, 0x5a};
	byte m2[] = {0xB5, 0x62, 0x06, 0x86, 0x00, 0x00, 0x8C, 0xAA};

	SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
	digitalWrite(GPS_CS, 0);
	for(int i = 0; i<16; i++){
		SPI.transfer(m1[i]);
	}
	digitalWrite(GPS_CS, 1);

	digitalWrite(GPS_CS, 0);
	for(int i = 0; i<8; i++){
		SPI.transfer(m2[i]);
	}
	digitalWrite(GPS_CS, 1);
}

void GPS_off(void){
	
	statVar.ena_vector &= ~ (ENA_GPS);
	byte m0[] = {0xB5, 0x62,   0x02, 0x41, 0x10, 0x00,    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,    0x06, 0x00, 0x00, 0x00,   0x80, 0x00, 0x00, 0x00,  0xD9, 0x4B};
	//byte m2[] = {0xB5, 0x62, 0x06, 0x86, 0x00, 0x00, 0x8C, 0xAA};
	
	byte m1[] = {0xB5, 0x62, 0x06, 0x57, 0x08, 0x00, 0x01, 0x00, 0x00, 0x00, 0x50, 0x4B, 0x43, 0x42, 0x86, 0x46};

	SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));


	//dummy send to turn on if was turned off, so it would capture the turn off command
	digitalWrite(GPS_CS, 0);
	SPI.transfer(0xff);
	digitalWrite(GPS_CS, 1);

	delay(500);
	digitalWrite(GPS_CS, 0);
	for(int i = 0; i<24; i++){
		SPI.transfer(m0[i]);
	}
	digitalWrite(GPS_CS, 1);
	
	digitalWrite(GPS_CS, 0);
	for(int i = 0; i<16; i++){
		SPI.transfer(m1[i]);
	}
	digitalWrite(GPS_CS, 1);

	//	digitalWrite(GPS_CS, 0);
	for(int i = 0; i<8; i++){
		//		SPI.transfer(m2[i]);
	}
	//	digitalWrite(GPS_CS, 1);
}

void GPS_stopped(void){
	
	
	byte m1[] = {0xB5, 0x62, 0x06, 0x57, 0x08, 0x00, 0x01, 0x00, 0x00, 0x00, 0x50, 0x4F, 0x54, 0x53, 0xAC, 0x85};
	byte m2[] = {0xB5, 0x62, 0x06, 0x57, 0x00, 0x00, 0x5d, 0x1d};

	SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
	digitalWrite(GPS_CS, 0);
	for(int i = 0; i<16; i++){
		SPI.transfer(m1[i]);
	}
	digitalWrite(GPS_CS, 1);

	digitalWrite(GPS_CS, 0);
	for(int i = 0; i<8; i++){
		SPI.transfer(m2[i]);
	}
	digitalWrite(GPS_CS, 1);
}


void disableGPSBckp(){
	digitalWrite(GPS_BCKP, 0);	
}


void gpsBckpTimer(){
	
	rtc.setAlarmEpoch(rtc.getEpoch()+(4*3600));  //time of ephemirides validity
	  rtc.enableAlarm(rtc.MATCH_HHMMSS);

	  rtc.attachInterrupt(disableGPSBckp);
	
}