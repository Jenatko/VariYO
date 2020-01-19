#include <Arduino.h>

#include <GxEPD2_BW.h>
//#include <GxGDEP015OC1/GxGDEP015OC1.h>
#include "powerModes.h"
#include "definitions.h"
#include "button_control.h"
#include "Interrupt_counter.h"
#include "wiring_private.h"
#include "EEPROM.h"
#include "Variables.h"
#include "SdFat.h"
#include "MEMS.h"
#include "RTCZero.h"
#include "MAX17055.h"

extern SdFile tracklog;
extern SdFile loggerFile;

extern SdFat SD;

void logger_routine(void){

	
	reinitializePins();

	

	
	pinMode(SD_DETECT, INPUT_PULLUP);
	delay(10);
	int ahoj = SD.begin(SD_CS, SD_SCK_MHZ(8));
	if(ahoj){
		pinMode(SD_DETECT, INPUT);
		float current = 0;
		request_si7021();
		if(present_devices & MAX17055_PRESENT){
			current = max17055.getAverageCurrent();
			battery_SOC = max17055.getSOC();
			battery_voltage = max17055.getAverageVoltage();
			if(battery_voltage < 3){
				rtc.detachInterrupt();
				logger_ena = 0;
				display.fillScreen(GxEPD_WHITE);
				display.setCursor(20, 100);
				display.print("Power Off");
				display.setCursor(10, 120);
				display.print("Battery empty");
				display.display();
			}
		}
		for(int timeout = 0; timeout < 10; timeout++){
			if(read_si7021()) break;
			else delay(10);
			if(timeout == 9) request_si7021();
		}
		request_si7021();
		for(int timeout = 0; timeout < 10; timeout++){
			if(read_si7021()) break;
			else delay(10);
		}
		loggerFile.open("logger.csv", O_WRONLY | O_APPEND | O_CREAT);
		if(loggerFile.isOpen()){
			loggerFile.print(rtc.getDay());
			loggerFile.print("/");
			loggerFile.print(rtc.getMonth());
			loggerFile.print("/20");
			
			loggerFile.print(rtc.getYear());
			loggerFile.print(" ");


			loggerFile.print(rtc.getHours());
			loggerFile.print(":");
			loggerFile.print(rtc.getMinutes());
			loggerFile.print(":");
			loggerFile.print(rtc.getSeconds());
			loggerFile.print(",");
			loggerFile.print(enviromental_data.temperature/100.0);
			loggerFile.print(",");
			loggerFile.print(enviromental_data.humidity/100.0);
			if(present_devices & MAX17055_PRESENT){
				loggerFile.print(",");
				loggerFile.print(battery_voltage);
				loggerFile.print(",");
				loggerFile.print(battery_SOC);
			}
			loggerFile.print(",");
			
			loggerFile.println(ahoj);
			loggerFile.sync();
			loggerFile.close();
		}
	}
	allLow();
	digitalWrite(GPS_BCKP, 0);
}

void powerOff(int lowVoltage, int GPS_BckpPwr) {
	if(statVar.ena_vector & ENA_TRACKLOG || tracklog_stat == TRACKLOG_FILE_OPEN){
		if(tracklog_stat == TRACKLOG_FILE_OPEN){
			tracklog.println("    </trkseg>");
			tracklog.println("  </trk>");
			tracklog.println("</gpx>");
			tracklog.close();
			tracklog_stat = TRACKLOG_FILE_CLOSED;
		}
		statVar.ena_vector &= ~(ENA_TRACKLOG);
	}
	
	

	display.fillScreen(GxEPD_WHITE);
	display.setCursor(20, 100);
	display.print("Power Off");
	if(lowVoltage){
		display.setCursor(10, 120);
		display.print("Battery empty");
	}
	display.display();
	while(digitalRead(DISP_BUSY));
	eepromWrite(0, statVar);
	USB->DEVICE.CTRLA.reg &= ~USB_CTRLA_ENABLE;
	//USB->DEVICE.CTRLA.reg &= ~USB_CTRLA_RUNSTDBY;
	SCB->SCR |= 1 << 2;
	EIC->WAKEUP.reg = EIC_WAKEUP_WAKEUPEN9;
	//display.powerDown();
	REG_TC4_INTENCLR = TC_INTENCLR_MC1 | TC_INTENCLR_MC0 | TC_INTENCLR_OVF;
	


	// pinMode(BUTTON_LEFT, OUTPUT);
	// pinMode(BUTTON_RIGHT, OUTPUT);
	// pinMode(BUTTON_UP, OUTPUT);
	// pinMode(BUTTON_DOWN, OUTPUT);
	// pinMode(BUTTON_CENTER, INPUT_PULLUP);
	
	allLow();
	if(GPS_BckpPwr) gpsBckpTimer();
	else	digitalWrite(GPS_BCKP, 0);
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
	
	REG_EIC_INTENCLR = EIC_INTENCLR_EXTINT9;
	REG_EIC_INTFLAG = EIC_INTFLAG_EXTINT9;
	attachInterrupt(BUTTON_CENTER, wakeUp, LOW);
	//__DSB();
	__WFI();
	//SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	for(int i = 0; i < 100; i++){
		if(digitalRead(BUTTON_CENTER)){   //go back to sleep if woken up by RTC timer (gps backup power turning off) or holding the button just for a while
			
			if(logger_ena){
				SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
				logger_routine();
			}
			//attachInterrupt(BUTTON_CENTER, wakeUp, LOW);
			SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
			attachInterrupt(BUTTON_CENTER, wakeUp, LOW);
			__WFI();

			i = 0;
		}
		delay(10);
	}
	logger_ena = 0;
	rtc.detachInterrupt();
	
	//comment when i am super happy, slightly reduces consumption
	USBDevice.init();
	USBDevice.attach();

	reinitializePins();

	BT_off();
	
	
	pinMode(SD_DETECT, INPUT_PULLUP);
	delay(10);
	if(digitalRead(SD_DETECT) == 0){
		present_devices |= SD_PRESENT;
		SD.begin(SD_CS, SD_SCK_MHZ(8));
	}
	else{
		present_devices &= ~SD_PRESENT;
	}
	pinMode(SD_DETECT, INPUT);


	
	display.init(0);


	display.setTextColor(GxEPD_BLACK);

	display.fillScreen(GxEPD_WHITE);

	//display.display();
	display.setRotation(0);

	display.display(true);
	display.setTextWrap(0);
	
	if(present_devices | BMX160_PRESENT)
	IMU_init();

	if(present_devices | LPS33_PRESENT)
	lps33_init();
	if(present_devices | SI7021_PRESENT)
	request_si7021();
	
	counterInit();
	
	if(statVar.ena_vector & (ENA_GPS)){
		if(statVar.ena_vector & (ENA_GPS_LOW_POWER)){
			GPS_low();
			GPS_low();
		}
		else{
			GPS_full();
			GPS_full();
		}
	}
	else{
		GPS_off();
		GPS_off();
	}
	
	while (buttons.getFlag()){
		buttons.getButtonPressed();
	}
	

}


void allLow() {
	
	//SPI.end();

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
	digitalWrite(POWER_ENA, POWER_OFF);
	digitalWrite(HEAT, 0);
	
	pinMode(BT_UART_TX, OUTPUT);  //BT uart tx
	digitalWrite(BT_UART_TX, 0);

	
}


void reinitializePins() {
	
	
	digitalWrite(GPS_BCKP, 1);
	pinMode(POWER_ENA, INPUT_PULLDOWN);
	//digitalWrite(POWER_ENA, 0);
	delay(150);
	Serial.begin(115200);
	pinMode(POWER_ENA, OUTPUT);
	digitalWrite(POWER_ENA, POWER_ON);
	digitalWrite(EEPROM_CS, 1);
	digitalWrite(SRAM_CS, 1);
	digitalWrite(SD_CS, 1);
	digitalWrite(SD_RST, 0);
	
	pinMode(SD_CS, OUTPUT);
	digitalWrite(SD_CS, 1);

	digitalWrite(SD_RST, 0);
	digitalWrite(IMU_CS, 1);
	digitalWrite(GPS_CS, 1);
	digitalWrite(BARO_CS, 1);
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

	SPI.begin();



}



void wakeUp() {
	
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;

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

	display.display();
	display.powerOff();
	
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


	SD.begin(SD_CS, SD_SCK_MHZ(8));

	
	display.init(0);
		display.fillScreen(GxEPD_WHITE);
		display.display(true);
	
}



void GPS_low(void){
	SerialUSB.println("gps low");
	
	statVar.ena_vector |= (ENA_GPS);
	statVar.ena_vector |= (ENA_GPS_LOW_POWER);
	byte m1[] = {0xB5, 0x62, 0x06, 0x86, 0x08, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x97, 0x6F};
	byte m2[] = {0xB5, 0x62, 0x06, 0x86, 0x00, 0x00, 0x8C, 0xAA};

	SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
	
	//dummy send to turn on if was turned off, so it would capture the next command
	digitalWrite(GPS_CS, 0);
	SPI.transfer(0xff);
	digitalWrite(GPS_CS, 1);
	delay(500);
	
	digitalWrite(GPS_CS, 0);
	for(int i = 0; i<16; i++){
		SPI.transfer(m1[i]);
	}
	digitalWrite(GPS_CS, 1);
	delay(10);
	digitalWrite(GPS_CS, 0);
	for(int i = 0; i<8; i++){
		SPI.transfer(m2[i]);
	}
	digitalWrite(GPS_CS, 1);
}

void GPS_full(void){
	SerialUSB.println("gps full");
	
	statVar.ena_vector |= (ENA_GPS);
	statVar.ena_vector &= ~(ENA_GPS_LOW_POWER);
	
	byte m1[] = {0xB5, 0x62, 0x06, 0x86, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x94, 0x5a};
	byte m2[] = {0xB5, 0x62, 0x06, 0x86, 0x00, 0x00, 0x8C, 0xAA};

	SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
	
	//dummy send to turn on if was turned off, so it would capture the next command
	digitalWrite(GPS_CS, 0);
	SPI.transfer(0xff);
	digitalWrite(GPS_CS, 1);
	delay(500);
	
	digitalWrite(GPS_CS, 0);
	for(int i = 0; i<16; i++){
		SPI.transfer(m1[i]);
	}
	digitalWrite(GPS_CS, 1);
	delay(10);
	digitalWrite(GPS_CS, 0);
	for(int i = 0; i<8; i++){
		SPI.transfer(m2[i]);
	}
	digitalWrite(GPS_CS, 1);
}

void GPS_off(void){
	SerialUSB.println("gps off");
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
	
	delay(10);
	
	digitalWrite(GPS_CS, 0);
	for(int i = 0; i<16; i++){
		SPI.transfer(m1[i]);
	}
	digitalWrite(GPS_CS, 1);

}

void GPS_stopped(void){
	
	statVar.ena_vector &= ~ (ENA_GPS);
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
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
	digitalWrite(GPS_BCKP, 0);
}


void gpsBckpTimer(){
	
	rtc.setAlarmEpoch(rtc.getEpoch()+(4*3600));  //time of ephemirides validity
	rtc.enableAlarm(rtc.MATCH_HHMMSS);

	rtc.attachInterrupt(disableGPSBckp);
	
}

void BT_off(void){
	pinMode(BT_UART_TX, OUTPUT);
	digitalWrite(BT_UART_TX, 0);
	delay(50);
	Serial.begin(115200);
	delay(100);   //restaart  bluetooth
	//Turn BT off
	Serial.write("$$$");
	delay(50);
	Serial.write("O,0");
}