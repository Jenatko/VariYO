

#include "SPI_IRQ.h"



#include <wiring_private.h>


#include "definitions.h"


#include "button_control.h"

#include "Menu.h"

#include "numpad.h"

#include "RTCZero.h"

#include "buzzer.h"

#include "roundbuff.h"

#include "routine.h"

#include "kalmanfilter3.h"

#include "MAX17055.h"

#include "EEPROM.h"

#include "Variables.h"

#include "powerModes.h"

#include <Wire.h>

#include "MEMS.h"





#include <GxEPD.h>
#include <GxGDEP015OC1/GxGDEP015OC1.h>    // 1.54" b/w
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

#include <Fonts/FreeMonoBold24pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>

#include "Interrupt_counter.h"


GxIO_Class io(SPI, /*CS=*/ DISP_CS, /*DC=*/ DISP_DC, /*RST=*/ DISP_RST);
GxEPD_Class display(io, /*RST=*/ DISP_RST, /*BUSY=*/ DISP_BUSY);






int pocitadlo = 0;


void Gauge_update(Gauge *gau);
void Gauge_enable(Gauge *gau);



void displayUpdate(void){
	display.fillRect(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_WHITE);
	
	statVar.varioGauge.value = vario_filter/100;
	Gauge_enable(&statVar.varioGauge);
	Gauge_update(&statVar.varioGauge);

	/*display.setCursor(20, 60);
	display.print("spd:");
	display.print(fix.speed_kph(),1);
	display.setCursor(20,80);
	display.print("hdg:");
	display.print(fix.heading(),1);
	*/
	display.setCursor(10,90);
	display.print(var_localtime->tm_hour);
	display.print(":");
	display.print(var_localtime->tm_min);
	display.print(":");
	display.print(var_localtime->tm_sec);
	display.setCursor(10,110);
	
	time_t flighttime_t = rtc.getEpoch() - var_takeofftime;
	struct tm *flighttime;
	flighttime = gmtime(&flighttime_t);
	display.print(flighttime->tm_hour);
	display.print(":");
	display.print(flighttime->tm_min);
	display.print(":");
	display.print(flighttime->tm_sec);
	
	display.setCursor(10,130);
	//display.print(alt_filter);
	display.print("wnd:");
	display.print(wind_speed_mps, 1);
	display.print("/");
	display.print(wind_direction, 0);
	
	
	display.setCursor(10,170);
	display.print("alt:");
	display.print(alt_filter/100);
	
	display.setCursor(10,150);
	display.print("spd:");
	display.print(fix.speed_kph(), 1);
	display.print("/");
	display.print(fix.heading(), 0);


	display.setFont(&FreeMonoBold9pt7b);
	
	display.setCursor(5, 190);
	display.print("temp:");
	display.print(enviromental_data.temperature/100.0);
	display.print("degC");
	

	/*
	display.setCursor(5, 197);
	display.print("y:");
	display.print(yaw, 0);
	display.setCursor(70, 197);
	display.print("p:");
	display.print(pitch, 0);
	display.setCursor(140, 197);
	display.print("r:");
	display.print(roll, 0);

	*/
	
	display.setFont(&FreeMonoBold12pt7b);
	display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, true);
	redraw = 0;
}


void setup() {



	rtc.begin();
	rtc.setTime(23, 59, 40);
	rtc.setDate(1, 1, 2014);
	
	pinMode(POWER_ENA, INPUT_PULLDOWN);
	
	delay(50);
	
	pinMode(BUTTON_LEFT, INPUT_PULLUP);
	pinMode(BUTTON_RIGHT, INPUT_PULLUP);
	pinMode(BUTTON_UP, INPUT_PULLUP);
	pinMode(BUTTON_DOWN, INPUT_PULLUP);
	pinMode(BUTTON_CENTER, INPUT_PULLUP);


	pinMode(EEPROM_CS, OUTPUT);
	digitalWrite(EEPROM_CS, 1);


	pinMode(SRAM_CS, OUTPUT);
	digitalWrite(SRAM_CS, 1);

	pinMode(SD_CS, OUTPUT);
	digitalWrite(SD_CS, 1);

	pinMode(SD_DETECT, INPUT);

	pinMode(SD_RST, OUTPUT);
	digitalWrite(SD_RST, 0);


	pinMode(IMU_CS, OUTPUT);
	digitalWrite(IMU_CS, 1);
	
	pinMode(BARO_CS, OUTPUT);
	digitalWrite(BARO_CS, 1);

	pinMode(GPS_CS, OUTPUT);
	digitalWrite(GPS_CS, 1);

	pinPeripheral(MOSI_IRQ, PIO_SERCOM);
	pinPeripheral(MISO_IRQ, PIO_SERCOM);
	pinPeripheral(SCK_IRQ, PIO_SERCOM);
	
	//clk_test();
	//buzzerEna(1);
	//delay(10000);


	SPI.begin();
	

	
	delay(100);   //wait for EEPROM to power-up
	
	
	
	eepromRead(0, statVar);
	


	
	if(statVar.ena_vector & (1<<ENA_GPS)){
		if(statVar.ena_vector & (1<<ENA_GPS_LOW_POWER)){
			GPS_low();
		}
		else{
			GPS_full();
		}
	}
	else{
		GPS_off();
	}
	
	menu_init();


	analogWrite(DAC, statVar.BuzzerVolume);
	analogWrite(DAC, 0);	//stfu for debug:-)
		analogWrite(DAC, 255);	//stfu for debug:-)


	display.init(0);

	display.setFont(&FreeMonoBold12pt7b);
	display.setTextColor(GxEPD_BLACK);

	display.fillRect(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_WHITE);
	display.update();
	display.setRotation(0);

	display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, false);
	display.setTextWrap(0);
	
	display.setCursor(100, 170);
	

	
	Wire.begin();

	max17055.setResistSensor(0.039);
	

	max17055.setCapacity(500);
	
	//	while(!SerialUSB.available());
	//	SerialUSB.println("wtf");

	IMU_init();
	BME_init_1ms();

	delay(100);
	

	int fn = 0;

	/*while (1)
	{	BME_read();
	SerialUSB.print(enviromental_data.temperature);
	if(fn%500 < 250){
	BME_init_1s();
	}
	else{
	BME_init_1s();
	}
	delay(1000);
	fn++;
	}*/
	
	
	
	buzzerInit();


	
	counterInit();
	
	delay(1000);
	
	



	kalmanFilter3_configure(1000.0, 10.0, 1.0, myRealAltitude, 0.0 , 0.0);
	
	
	statVar.varioGauge.size_X = 130;
	statVar.varioGauge.size_Y = 40;
	statVar.varioGauge.offset_X = 10;
	statVar.varioGauge.offset_Y = 30;
	statVar.varioGauge.font = 2;
	statVar.varioGauge.frame = 1;
	statVar.varioGauge.decimals = 1;
	statVar.varioGauge.ena = 1;
	statVar.varioGauge.value = 1.5;
	String("Vario").toCharArray(statVar.varioGauge.name_shown, 10);
	String("m/s").toCharArray(statVar.varioGauge.units, 4);
	



}

void loop() {
	





	if (buttons.getFlag()){
		switch (buttons.getButtonPressed()){
			case PRESS:
			MenuEntry(&topmenu);
			display.fillRect(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_WHITE);
			display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, false);

			display.update();
			break;
			
			case DOWN:
			statVar.BuzzerVolume -=50;
			if(statVar.BuzzerVolume<0){
				statVar.BuzzerVolume = 0;
			}
			analogWrite(DAC, statVar.BuzzerVolume);
			SerialUSB.println("down_vol");
			break;
			
			case UP:
			statVar.BuzzerVolume +=50;
			if(statVar.BuzzerVolume>255){
				statVar.BuzzerVolume = 255;
			}
			analogWrite(DAC, statVar.BuzzerVolume);
			SerialUSB.println("up_vol");
			break;
		}

	}


	routine();


	if(redraw){

		/*
		Wire.beginTransmission(SI7021_ADDRESS);
		Wire.write(SI7021_MEASURE_RH);
		Wire.endTransmission();
		int byte = 0;
		delay(15);
		while(!byte){
		byte = Wire.requestFrom(SI7021_ADDRESS, 2);
		}
		int rh = 0;
		rh = Wire.read()<<8;
		rh += Wire.read();
		SerialUSB.print((125.0*rh/65536.0)-6.0);
		SerialUSB.print(',');
		
		
		
		Wire.beginTransmission(SI7021_ADDRESS);
		Wire.write(SI7021_READ_TEMP_FROM_RH);
		Wire.endTransmission();
		byte = 0;
		delay(15);
		while(!byte){
		byte = Wire.requestFrom(SI7021_ADDRESS, 2);
		}
		rh = 0;
		rh = Wire.read()<<8;
		rh += Wire.read();
		SerialUSB.println((175.72*rh/65536.0)-46.85);
		
		*/
		
		//	SerialUSB.println(rtc.getEpoch());
		//   unsigned long fn = micros();
		time_t tempTime = rtc.getEpoch();
		tempTime += 3600 * statVar.TimeZone;
		var_localtime = gmtime(&tempTime);
		//	unsigned long fn2 = micros();
		//	SerialUSB.println(fn2-fn);		//time check, takes around 5ms
		
		//	SerialUSB.println(mktime(var_localtime));
		
		displayUpdate();
		
		if(max17055.getAverageVoltage() < 2.95){
			powerOff();
		}
		

		
		/*
		if(pocitadlo % 10 == 0){
		SerialUSB.println(enviromental_data.temperature);
		
		SerialUSB.print("time;");
		SerialUSB.print(pocitadlo);
		SerialUSB.print(";Vavg;");
		SerialUSB.print(max17055.getAverageVoltage(), 3); display.print(" V");
		SerialUSB.print(";Iavg;");
		SerialUSB.print(max17055.getAverageCurrent());  display.print(" mA");
		SerialUSB.print(";SOC;");
		SerialUSB.print(max17055.getSOC());  display.print(" %");
		SerialUSB.print(";TTE;");
		SerialUSB.print(max17055.getTimeToEmpty()); display.print(" hr");
		SerialUSB.print(";TTF;");
		SerialUSB.print(max17055.getTimeToFull()); display.print(" hr");
		SerialUSB.print(";measCap;");
		SerialUSB.print(max17055.getReportedCapacity()); display.print(" mAH");
		SerialUSB.print(";remCap;");
		SerialUSB.println(max17055.getRemainingCapacity()); display.print(" mAH");
		
		
		}*/

		
		

		//	pocitadlo ++;
		
		
	}
	else{
		delay(100);
	}
	
	
	
	
	
	

}



void Gauge_enable(Gauge *gau) {
	if (gau->ena == 1) {
		if (gau->frame == 1) {
			display.drawRect(gau->offset_X, gau->offset_Y, gau->size_X, gau->size_Y, GxEPD_BLACK);
			//      display.fillRect(0, 0, 199, 199, GxEPD_BLACK);
			//         display.drawRect(offset_X-1, offset_Y-1, size_X+1, size_Y+1, GxEPD_WHITE);
		}
		if (gau->name_shown[0] != '\0') {
			display.fillRect(gau->offset_X + 5 , gau->offset_Y - 11, strlen(gau->name_shown) * 12, 13, GxEPD_WHITE);
			display.setCursor(gau->offset_X + 8, gau->offset_Y);
			display.setFont(&FreeMonoBold9pt7b);

			display.print(gau->name_shown);

			display.setFont(&FreeMonoBold12pt7b);

		}
		if (gau->units[0] != '\0') {
			display.setFont(&FreeMonoBold9pt7b);
			switch (gau->font) {
				case (0):
				display.setCursor(gau->offset_X + 5, gau->offset_Y + 20);
				break;
				case (1):
				display.setCursor(gau->offset_X + 5, gau->offset_Y + 20);
				break;
				case (2):
				display.setCursor(gau->offset_X + 5, gau->offset_Y + 25);
				break;
				case (3):
				display.setCursor(gau->offset_X + 5, gau->offset_Y + 32);
				break;
			}
			display.setCursor(gau->offset_X + gau->size_X - strlen(gau->units) * 12, gau->offset_Y + gau->size_Y - 17);
			display.print(gau->units);
			display.setFont(&FreeMonoBold12pt7b);

		}


	}
}




void Gauge_update(Gauge *gau) {
	switch (gau->font) {
		case (0):
		display.setFont(&FreeMonoBold9pt7b);
		display.setCursor(gau->offset_X + 5, gau->offset_Y + 20);
		break;
		case (1):
		display.setFont(&FreeMonoBold12pt7b);
		display.setCursor(gau->offset_X + 5, gau->offset_Y + 22);
		break;
		case (2):
		display.setFont(&FreeMonoBold18pt7b);
		display.setCursor(gau->offset_X + 5, gau->offset_Y + 28);
		break;
		case (3):
		display.setFont(&FreeMonoBold24pt7b);
		display.setCursor(gau->offset_X + 5, gau->offset_Y + 35);
		break;
	}
	display.fillRect(gau->offset_X + 1, gau->offset_Y + 1, gau->size_X - 2 - strlen(gau->units) * 12, gau->size_Y - 2, GxEPD_WHITE);
	//display.setCursor(offset_X + 5, offset_Y + 22);
	if(gau->value >=0)
	display.print("+");
	display.print(gau->value, gau->decimals);
	display.setFont(&FreeMonoBold12pt7b);
}


