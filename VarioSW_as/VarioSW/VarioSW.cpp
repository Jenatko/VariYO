

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

#include "chess.h"

#include "routine.h"


// #include <GxEPD.h>
// #include <GxGDEP015OC1/GxGDEP015OC1.h>    // 1.54" b/w
// #include <GxIO/GxIO_SPI/GxIO_SPI.h>
// #include <GxIO/GxIO.h>

#define ENABLE_GxEPD2_GFX 0


#include <GxEPD2_BW.h>
#define MAX_DISPAY_BUFFER_SIZE 15000ul // ~15k is a good compromise
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= MAX_DISPAY_BUFFER_SIZE / (EPD::WIDTH / 8) ? EPD::HEIGHT : MAX_DISPAY_BUFFER_SIZE / (EPD::WIDTH / 8))



#include <Fonts/FreeMonoBold24pt7b.h>
#include <Fonts/FreeMonoBold18pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>

#include "Interrupt_counter.h"

GxEPD2_BW<GxEPD2_154, GxEPD2_154::HEIGHT> display(GxEPD2_154(/*CS=D8*/ PA13, /*DC=D3*/ PA14, /*RST=D4*/ PA15, /*BUSY=D2*/ PA10));


//GxIO_Class io(SPI, /*CS=*/ DISP_CS, /*DC=*/ DISP_DC, /*RST=*/ DISP_RST);
//GxEPD_Class display(io, /*RST=*/ DISP_RST, /*BUSY=*/ DISP_BUSY);


#include <SD.h>





volatile int counter_incremented_every_ISR = 0;




void draw_antenna(int x_oirg, int y_orig);
void draw_floppy(int x_orig, int y_orig);





void displayUpdate(void){


	display.fillScreen(GxEPD_WHITE);


	statVar.varioGauge.value = vario_filter/100;
		statVar.varioAvgGauge.value = vario_lowpassed/100;
	statVar.AGLGauge.value = (alt_filter/100) - ground_level;
	statVar.altitudeGauge.value = (alt_filter/100);
	statVar.tempGauge.value = enviromental_data.temperature/100;
	statVar.humidGauge.value = enviromental_data.humidity/100;
	statVar.speedGauge.value = fix.speed_kph();
	statVar.headingGauge.value = fix.heading();
	statVar.windGauge.value = wind_speed_mps;
	statVar.windDirGauge.value = wind_direction;
	
	printGauges();
	
	


	display.setFont(&FreeMonoBold9pt7b);
	display.setCursor(5,12);
	if(var_localtime.tm_hour < 10) display.print("0");
	display.print(var_localtime.tm_hour);
	display.print(":");
	if(var_localtime.tm_min < 10) display.print("0");
	display.print(var_localtime.tm_min);
	display.print(":");
	if(var_localtime.tm_sec < 10) display.print("0");
	display.print(var_localtime.tm_sec);

	
	display.setFont();
	
	draw_antenna(100, 0);
	display.setCursor(113,5);
	//	display.setFont();
	if(statVar.ena_vector & ENA_GPS){
		if(fix.valid.location)
		display.print("3D");
		else if(fix.valid.time)
		display.print("time");
		else
		display.print("No");
	}
	else		display.print("OFF");
	
	if(statVar.ena_vector & ENA_TRACKLOG)	draw_floppy(150, 0);
	
	
	
	
	display.setCursor(176,3);

	display.print(battery_SOC, 0);
	display.print("%");
	display.drawLine(170, 0, 198, 0, GxEPD_BLACK);
	display.drawLine(170, 12, 198, 12, GxEPD_BLACK);
	display.drawLine(170, 0, 170, 12, GxEPD_BLACK);
	display.drawLine(198, 0, 198, 12, GxEPD_BLACK);
	display.drawLine(169, 3, 169, 9, GxEPD_BLACK);
	display.drawLine(168, 3, 168, 9, GxEPD_BLACK);
	
	display.setCursor(10,110);
	display.setFont(&FreeMonoBold12pt7b);
	

	
	display.setFont(&FreeMonoBold12pt7b);

	
	//	display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, true);
	
	//display.partiallyUpdateScreen();
	display.display(true);
	

	redraw = 0;
	
}


void setup() {



	rtc.begin();
	rtc.setTime(23, 59, 40);
	rtc.setDate(1, 1, 2014);
	
	pinMode(HEAT, OUTPUT);
	digitalWrite(HEAT, 0);
	
	pinMode(POWER_ENA, OUTPUT);
	digitalWrite(POWER_ENA, 1);
	
	delay(100);
	digitalWrite(POWER_ENA, 0);
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

	pinMode(SD_DETECT, INPUT_PULLUP);

	pinMode(SD_RST, OUTPUT);
	digitalWrite(SD_RST, 0);


	pinMode(IMU_CS, OUTPUT);
	digitalWrite(IMU_CS, 1);
	
	pinMode(BARO_CS, OUTPUT);
	digitalWrite(BARO_CS, 1);

	pinMode(GPS_CS, OUTPUT);
	digitalWrite(GPS_CS, 1);
	
	pinMode(GPS_BCKP, OUTPUT);
	digitalWrite(GPS_BCKP, 1);

	pinPeripheral(MOSI_IRQ, PIO_SERCOM);
	pinPeripheral(MISO_IRQ, PIO_SERCOM);
	pinPeripheral(SCK_IRQ, PIO_SERCOM);
	
	

	//clk_test();
	//buzzerEna(1);
	//delay(10000);


	SPI.begin();
	

	
	delay(100);   //wait for EEPROM to power-up
	
	
	
	eepromRead(0, statVar);
	
	present_devices = 0;
	
	


	
	if(statVar.ena_vector & (ENA_GPS)){
		if(statVar.ena_vector & (ENA_GPS_LOW_POWER)){
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
	//while(!SerialUSB.available());


	analogWrite(DAC, statVar.BuzzerVolume);
	Wire.begin();

	display.init(0);


	display.setFont(&FreeMonoBold12pt7b);
	display.setTextColor(GxEPD_BLACK);

	display.fillRect(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_WHITE);
	display.display();
	
	display.setRotation(0);

	display.display(true);

	display.setTextWrap(0);
	
	display.setCursor(10, 20);
	display.print("display ok");
	display.display(true);

	

	
	display.setCursor(10, 40);
	if(max17055.checkFunct()){
		max17055.setResistSensor(0.039);
		max17055.setCapacity(1200);
		display.print("max17055 ok");
		present_devices |= MAX17055_PRESENT;
		
	}
	else{
		display.print("max17055 NOK");
	}
	display.display(true);

	

	display.setCursor(10, 60);
	if(IMU_init()==0){
		display.print("BMX160 ok");
		present_devices |= BMX160_PRESENT;
		//SerialUSB.println( MAG_init());
	}
	else{
		display.print("BMX160 NOK");
	}
	//present_devices += BMX160_PRESENT;
	display.display(true);
	
	
	display.setCursor(10, 80);
	if(lps33_test()){
		lps33_init();
		display.print("lps33 ok");
		present_devices |= LPS33_PRESENT;
	}
	else{
		display.print("lps33 NOK");
	}
	display.display(true);
	
	
	display.setCursor(10, 100);
	if(request_si7021() == 0){
		display.print("si7021 ok");
		present_devices |= SI7021_PRESENT;
	}
	else{
		display.print("si7021 NOK");
	}
	display.display(true);
	
	display.setCursor(10, 120);
	if(digitalRead(SD_DETECT) == 0){
		display.print("SD present");
		present_devices |= SD_PRESENT;
		SD.begin(SD_CS);
	}
	else{
		display.print("no SD card");
	}
	pinMode(SD_DETECT, INPUT);
	display.display(true);
	


	delay(1000);
	
	
	
	buzzerInit();


	
	counterInit();
	
	delay(1000);
	
	
//setVariablesDefault();


	kalmanFilter3_configure(statVar.zvariance, statVar.accelvariance, 1.0, alt_baro, 0.0 , 0.0);
	
	vario_lowpassed = 0;

	
	
	// 	while(!SerialUSB.available());
	//
	// 	for(int i = -201; i < 200; i +=50){
	// 		SerialUSB.print(i);
	// 		SerialUSB.print(",");
	// 		alt_agl_debug((float)i/100, 16.500f);
	// 	}



}



void loop() {


	if (buttons.getFlag()){
		switch (buttons.getButtonPressed()){
			case PRESS:
			MenuEntry(&topmenu);
			display.fillScreen(GxEPD_WHITE);
			//display.fillRect(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_WHITE);
			//display.display(true);
			display.display();
			break;
			
			case DOWN:
			statVar.BuzzerVolume -=30;
			if(statVar.BuzzerVolume<=0){
				statVar.BuzzerVolume = 0;
				
			}
			buzzerSetVolume(statVar.BuzzerVolume);
			break;
			
			case UP:
			statVar.BuzzerVolume +=30;
			if(statVar.BuzzerVolume>255){
				statVar.BuzzerVolume = 255;
				
			}
			buzzerSetVolume(statVar.BuzzerVolume);
			break;
		}

	}
	
	uint64_t t_start_first = micros();
	routine(0);
	uint64_t t_stop_first = micros();
	//	SerialUSB.println((uint32_t)((t_stop-t_start)/1000));



	if(redraw){
		//digitalWrite(SRAM_CS, 0);
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
		var_localtime = *gmtime(&tempTime);
		
		//	unsigned long fn2 = micros();
		//	SerialUSB.println(fn2-fn);		//time check, takes around 5ms
		
		//	SerialUSB.println(mktime(var_localtime));
		t_start_first = micros();
		displayUpdate();
		t_stop_first = micros();
		//SerialUSB.print((uint32_t)((t_stop_first-t_start_first)/1000));

		
		
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
	else if(counter500ms>28 && counter500ms < 100){
		uint64_t t_start_second = micros();
		displayUpdate();
		uint64_t t_stop_second = micros();
		counter500ms = 1000;
		//SerialUSB.print((uint32_t)((t_stop_first-t_start_first)/1000));
		//	SerialUSB.print(",");
		//	SerialUSB.println((uint32_t)((t_stop_second-t_start_second)/1000));
		//routine();
		
	}
	else{
		//SerialUSB.println(counter500ms);
		delay(10);

	}
	
	
	
	
	
	

}


void draw_antenna(int x_orig, int y_orig){
	display.drawLine(x_orig + 4, y_orig, x_orig + 7, y_orig, GxEPD_BLACK);
	display.drawLine(x_orig, y_orig+4, x_orig, y_orig+7, GxEPD_BLACK);
	display.drawLine(x_orig+1, y_orig+3, x_orig+3, y_orig+1, GxEPD_BLACK);
	
	display.drawLine(x_orig+2, y_orig+5, x_orig+5, y_orig+2, GxEPD_BLACK);
	display.drawPixel(x_orig+2, y_orig+6, GxEPD_BLACK);
	display.drawPixel(x_orig+6, y_orig+2, GxEPD_BLACK);
	
	display.drawLine(x_orig+6, y_orig+4, x_orig+4, y_orig+6, GxEPD_BLACK);
	
	display.drawLine(x_orig+10, y_orig+3, x_orig+3, y_orig+10, GxEPD_BLACK);
	display.drawLine(x_orig+10, y_orig+4, x_orig+10, y_orig+7, GxEPD_BLACK);
	display.drawLine(x_orig+4, y_orig+10, x_orig+7, y_orig+10, GxEPD_BLACK);
	
	display.drawPixel(x_orig+11, y_orig+11, GxEPD_BLACK);
	display.drawPixel(x_orig+10, y_orig+10, GxEPD_BLACK);
	display.drawPixel(x_orig+10, y_orig+11, GxEPD_BLACK);
	display.drawPixel(x_orig+9, y_orig+11, GxEPD_BLACK);
	display.drawPixel(x_orig+9, y_orig+9, GxEPD_BLACK);
	display.drawPixel(x_orig+9, y_orig+8, GxEPD_BLACK);
	display.drawPixel(x_orig+8, y_orig+9, GxEPD_BLACK);
	display.drawPixel(x_orig+6, y_orig+6, GxEPD_BLACK);
	display.drawPixel(x_orig+11, y_orig+12, GxEPD_BLACK);
	display.drawPixel(x_orig+10, y_orig+12, GxEPD_BLACK);
	display.drawPixel(x_orig+9, y_orig+12, GxEPD_BLACK);
}

void draw_floppy(int x_orig, int y_orig){
	display.drawLine(x_orig, y_orig, x_orig + 11, y_orig, GxEPD_BLACK);
	display.drawLine(x_orig, y_orig, x_orig, y_orig+10, GxEPD_BLACK);
	display.drawLine(x_orig+11, y_orig, x_orig + 11, y_orig+11, GxEPD_BLACK);
	display.drawLine(x_orig+1, y_orig+11, x_orig + 11, y_orig+11, GxEPD_BLACK);
	display.drawLine(x_orig+2, y_orig+1, x_orig + 2, y_orig+6, GxEPD_BLACK);
	display.drawLine(x_orig+9, y_orig+1, x_orig + 9, y_orig+6, GxEPD_BLACK);
	display.drawLine(x_orig+2, y_orig+6, x_orig + 9, y_orig+6, GxEPD_BLACK);
	
	display.drawLine(x_orig+3, y_orig+8, x_orig + 8, y_orig+8, GxEPD_BLACK);
	display.drawPixel(x_orig+3, y_orig+9, GxEPD_BLACK);
	display.drawPixel(x_orig+3, y_orig+10, GxEPD_BLACK);
	display.drawPixel(x_orig+6, y_orig+9, GxEPD_BLACK);
	display.drawPixel(x_orig+6, y_orig+10, GxEPD_BLACK);
	display.drawPixel(x_orig+7, y_orig+10, GxEPD_BLACK);
	display.drawPixel(x_orig+7, y_orig+9, GxEPD_BLACK);
	display.drawPixel(x_orig+8, y_orig+9, GxEPD_BLACK);
	display.drawPixel(x_orig+8, y_orig+10, GxEPD_BLACK);

}

