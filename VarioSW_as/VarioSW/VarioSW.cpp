
#include <Arduino.h>
#include "SPI.h"
#include "SPI_IRQ.h"

#include "MS5611.h"

#include "BMI160Gen.h"

#include <wiring_private.h>


#include "definitions.h"


#include "button_control.h"

#include "Menu.h"

#include "numpad.h"

#include "RTCZero.h"

#include "buzzer.h"

#include "roundbuff.h"


#include <SD.h>


#include <NMEAGPS.h>
#include <GPSport.h>
#include <Streamers.h>
static NMEAGPS  gps;
static gps_fix  fix;


#include <GxEPD.h>
#include <GxGDEP015OC1/GxGDEP015OC1.h>    // 1.54" b/w
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

#include <Fonts/FreeMonoBold24pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>

#include "Interrupt_counter.h"


GxIO_Class io(SPI, /*CS=*/ 38, /*DC=*/ 2, /*RST=*/ 5);
GxEPD_Class display(io, /*RST=*/ 5, /*BUSY=*/ 3);



Sd2Card card;
SdVolume volume;
SdFile root;

File tracklog;
int trackloggin = 0;

int th_rise = 30;
int th_sink = -150;
double myRealAltitude = 230;

int tracklog_ena = 0;

volatile float altChange = 0;


void setup() {
	

	
	rtc.begin();
	rtc.setTime(23, 59, 40);
	rtc.setDate(1, 1, 2014);
	
	menu_init();
	
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

	pinMode(SD_OE, OUTPUT);
	digitalWrite(SD_OE, 1);

	pinMode(GPS_PPS, INPUT);




	pinMode(IMU_CS, OUTPUT);
	digitalWrite(IMU_CS, 1);
	


	pinMode(UNUSED2, OUTPUT);
	digitalWrite(UNUSED2, 1);
	pinMode(UNUSED3, OUTPUT);
	digitalWrite(UNUSED3, 1);
	pinMode(UNUSED4, OUTPUT);
	digitalWrite(UNUSED4, 1);


	pinMode(GPS_CS, OUTPUT);
	digitalWrite(GPS_CS, 1);
	pinMode(GPS_UART_TX, OUTPUT);
	digitalWrite(GPS_UART_TX, 1);


	pinPeripheral(10, PIO_SERCOM);
	pinPeripheral(11, PIO_SERCOM);
	pinPeripheral(13, PIO_SERCOM);




	SPI.begin();

	
	
	
	



	//SerialUSB.begin(9600);
	
	ms5611.begin(MS5611_ULTRA_HIGH_RES);
	

	

	BMI160.begin(BMI160GenClass::SPI_MODE, IMU_CS);

	
	BMI160.autoCalibrateGyroOffset();
	BMI160.setGyroRate(25);
	BMI160.setAccelerometerRate(25);
	BMI160.setGyroFIFOEnabled(1);
	BMI160.setAccelFIFOEnabled(1);
	BMI160.setFIFOHeaderModeEnabled(0);
	BMI160.resetFIFO();
	
	//   BMI160.suspendAll();
	
	display.init(0);

	display.setFont(&FreeMonoBold12pt7b);
	display.setTextColor(GxEPD_BLACK);

	display.fillRect(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_WHITE);
	display.update();
	display.setRotation(0);

	display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, false);
	display.setTextWrap(0);
	
	display.setCursor(100, 170);


	//	SerialUSB.begin(9600);
	Serial1.begin(9600);
	
	
	
	

	
	counterInit();
	
	buzzerInit();
	buzzerFreq(1000);
	buzzerEna(1);



}

void loop() {

	if (buttons.getFlag())
	if (buttons.getButtonPressed() == 4){
		drawMenu(&topmenu);
		display.fillRect(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_WHITE);
		display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, false);

		display.update();
	}


	
	//gps test
	
	SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
	digitalWrite(GPS_CS, 0);
	byte fn;
	do {
		fn = SPI.transfer( 0xFF );
		//	SerialUSB.write(fn);
	} while (gps.handle(fn) != NMEAGPS::DECODE_CHR_INVALID || fn != 0xff );
	
	digitalWrite(GPS_CS, 1);
	if(gps.available()){
		fix = gps.read();
		SerialUSB.println("fixupdate");
		
		
		//SerialUSB.println(fix.latitude(), 6);
		//SerialUSB.print(fix.dateTime.hours);
		//SerialUSB.print(":");
		//SerialUSB.print(fix.dateTime.minutes);
		//SerialUSB.print(":");
		//SerialUSB.println(fix.dateTime.seconds);
		
		display.fillRect(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_WHITE);
		display.setCursor(20, 20);
		display.print("lat:");
		display.print(fix.latitude(), 5);
		display.setCursor(20, 40);
		display.print("lon:");
		display.print(fix.longitude(),5);
		display.setCursor(20, 60);
		display.print("spd:");
		display.print(fix.speed_kph(),1);
		display.setCursor(20,80);
		display.print("hdg:");
		display.print(fix.heading(),1);
		display.setCursor(20,100);
		display.print(fix.dateTime.hours);
		display.print(":");
		display.print(fix.dateTime.minutes);
		display.print(":");
		display.print(fix.dateTime.seconds);
		display.setCursor(20,120);
		display.print(ms5611.readAltitude(rbuff.getAverage()));
		
		if(fix.valid.time){
			rtc.setTime(fix.dateTime.hours, fix.dateTime.minutes, fix.dateTime.seconds);
			rtc.setDate(fix.dateTime.date, fix.dateTime.month, fix.dateTime.year);
		}
		display.setCursor(20, 170);
		
		display.print(altChange, 1);
		
		display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, true);

		if (tracklog_ena == 1)
		{   SerialUSB.println("opening");
			
			char cesta_char[40];


			SD.begin(SD_CS);
			
			String cesta = ("/Tracks/");
			cesta.concat(String(rtc.getYear()));
			cesta.concat("/");
			cesta.concat(String(rtc.getMonth()));
			cesta.concat("/");
			cesta.concat(String(rtc.getDay()));
	
			cesta.toCharArray(cesta_char, 40);
			
			if(SD.exists(cesta_char))
				SerialUSB.println("existuje");
		    else{
				SerialUSB.println(cesta_char);
				SD.mkdir(cesta_char);
				SerialUSB.println("vytvoreno");
			}
			
			cesta.concat("/");
			cesta.concat(String(rtc.getHours()));
						cesta.concat(String(rtc.getMinutes()));
						cesta.concat(String(rtc.getSeconds()));
						cesta.concat(String(".gpx"));
						cesta.toCharArray(cesta_char, 40);

			tracklog = SD.open(cesta_char, FILE_WRITE);

			tracklog.println("<gpx>");
			tracklog.println("  <trk>");
			tracklog.println("    <trkseg>");
			tracklog_ena = 2;

		}
		if (tracklog){
			SerialUSB.println("logging");
			if(fix.valid.location){
				//if(1){
				tracklog.print("      <trkpt lat=\"");
				tracklog.print(fix.latitude(), 6);
				tracklog.print("\" lon=\"");
				tracklog.print(fix.longitude(),5);
				tracklog.println("\">");
				
				tracklog.print("        <ele>");
				tracklog.print(fix.altitude(), 1);
				tracklog.println("</ele>");
				
				tracklog.print("        <time>");
				
				tracklog.print(fix.dateTime.year);
				tracklog.print("-");
				tracklog.print(fix.dateTime.month);
				tracklog.print("-");
				tracklog.print(fix.dateTime.date);
				tracklog.print("T");
				
				tracklog.print(fix.dateTime.hours);
				tracklog.print(":");
				tracklog.print(fix.dateTime.minutes);
				tracklog.print(":");
				tracklog.print(fix.dateTime.seconds);
				
				tracklog.println("Z</time>");
				tracklog.println("      </trkpt>");
			}
			
			if(tracklog_ena == 0){
				SerialUSB.println("closing");
				tracklog.println("    </trkseg>");
				tracklog.println("  </trk>");
				tracklog.println("</gpx>");
				tracklog.close();
				
			}
		}
		
		
		
	}
	else
	delay(100);



	
	
	
	//test IMU + baro
	/*
	display.fillRect(10, 5, 150, 180, GxEPD_WHITE);
	display.setCursor(20, 45);
	display.print(ms5611.readTemperature(), 1);
	display.print(" degC");



	display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, true);
	
	ms5611.tempUpdate(1);
	delay(100);
	ms5611.tempUpdate(2);
	
	while (BMI160.getFIFOCount() != 0) {


	uint8_t pole[13];
	
	BMI160.getFIFOBytes(pole, 12);
	//   SerialUSB.println(BMI160.getFIFOCount());

	int accel[3];
	int gyro[3];
	gyro[0] = pole[0] | ((int8_t)pole[1] << 8);
	gyro[1] = pole[2] | ((int8_t)pole[3] << 8);
	gyro[2] = pole[4] | ((int8_t)pole[5] << 8);

	accel[0] = pole[6] | ((int8_t)pole[7] << 8);
	accel[1] = pole[8] | ((int8_t)pole[9] << 8);
	accel[2] = pole[10] | ((int8_t)pole[11] << 8);
	
	//	for (int i = 0; i < 11; i++) {
	//		SerialUSB.print(pole[i]);
	//		SerialUSB.print(";");
	//	}
	
	SerialUSB.print(gyro[0]);
	SerialUSB.print(";");
	SerialUSB.print(gyro[1]);
	SerialUSB.print(";");
	SerialUSB.print(gyro[2]);
	SerialUSB.print(";");
	SerialUSB.print(accel[0]);
	SerialUSB.print(";");
	SerialUSB.print(accel[1]);
	SerialUSB.print(";");
	SerialUSB.println(accel[2]);
	
	

	}
	*/
	
}
