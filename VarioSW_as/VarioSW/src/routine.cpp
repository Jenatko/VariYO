/*
* routine.cpp
*
* Created: 16/10/2018 22:39:40
*  Author: Jena
*/
#include "routine.h"
#include "definitions.h"
#include <Arduino.h>
#include "SPI.h"
#include "RTCZero.h"
#include "MEMS.h"
#include "MAX17055.h"

#include "powerModes.h"

#include <NMEAGPS.h>
#include <GPSport.h>
#include <Streamers.h>

#include "Variables.h"

#include "CircleFitByTaubin.h"

#include "MadgwickAHRS.h"


NMEAGPS  gps;
gps_fix  fix;

#include <SD.h>
Sd2Card card;
SdVolume volume;
SdFile root;
File tracklog;
File HeightData;




int sec_old = 0;
int lat_last, lon_last, filenotpresent;



//uint8_t var_update_wind = 0;
//uint8_t var_update_tracklog = 0;

/*
void adjustTime( NeoGPS::time_t & dt ){
int32_t          zone_hours   = statVar.TimeZone; // EST
int32_t          zone_minutes =  0L; // usually zero
NeoGPS::clock_t  zone_offset  =	zone_hours   * NeoGPS::SECONDS_PER_HOUR +	zone_minutes * NeoGPS::SECONDS_PER_MINUTE;

NeoGPS::clock_t seconds = dt;
seconds += zone_offset;
dt = seconds;
}
*/

//void routine(int pollGPS = 1, int performOtherTasks = 1)
void routine(int OnlyReadGPS){


	//updating GPS
	if(statVar.ena_vector & ENA_GPS){
		SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
		digitalWrite(GPS_CS, 0);
		byte fn;
		do {
			fn = SPI.transfer( 0xFF );
			//SerialUSB.write(fn);   //uncomment for sending GPS data over Serial (to work with u-center)
		} while (gps.handle(fn) != NMEAGPS::DECODE_CHR_INVALID || fn != 0xff );
		
		digitalWrite(GPS_CS, 1);
		if(gps.available()){
			fix = gps.read();
			//SerialUSB.println(fix.valid.date);
			if(fix.valid.time){
				//adjustTime(fix.dateTime);
				rtc.setTime(fix.dateTime.hours, fix.dateTime.minutes, fix.dateTime.seconds);
			}
			if(fix.valid.date){
				rtc.setDate(fix.dateTime.date, fix.dateTime.month, fix.dateTime.year);
			}
			

			//var_update_tracklog = 1;
			//var_update_wind = 1;
			
			
			
			redraw = 1;
			counter500ms = 0;
			position_updated = 1;
		}
		else{
			position_updated = 0;
		}
		//procedure for capturing commands sent to GPS

		// 		SerialUSB.println("-----------------");
		// 		while(SerialUSB.available()) {      // If anything comes in Serial (USB),
		// 			SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
		// 			digitalWrite(GPS_CS, 0);
		// 			fn = SerialUSB.read();
		// 			gps.handle(SPI.transfer(fn));
		// 			SerialUSB.print(fn);
		// 			digitalWrite(GPS_CS, 1);
		//
		// 		}
		// 		SerialUSB.println("-----------------");
		// 		delay(50);

		
	}
	//update display if GPS is off
	else{
		int sec = rtc.getSeconds();
		if( sec != sec_old){
			redraw = 1;
			sec_old = sec;
			counter500ms = 0;
		}
		
		position_updated = 0;
	}

	if(OnlyReadGPS == 0){
		if(position_updated){
			alt_agl();
			update_wind();

		}
		
		update_tracklog();


		g_meter = pow(  ((float)ax*ax + (float)ay*ay + (float)az*az)/268.4e6 , 0.5) ;
		
		if(present_devices & SI7021_PRESENT){
			if(read_si7021())
			request_si7021();
		}
		
		if(present_devices & MAX17055_PRESENT){
			battery_SOC = max17055.getSOC();
			battery_voltage = max17055.getAverageVoltage();
			
			if(battery_voltage < 3){
				powerOff(1);
				
			}
		}
		yaw = Madgwick_filter.getYaw();
		pitch = Madgwick_filter.getPitch();
		roll = Madgwick_filter.getRoll();
	}

}


//so far not used since there is plenty of time while refreshing display. needs to prevent writing duplicate points in tracklog if GPS is not polled every time before writing tracklog
void redrawRoutine(){
	SerialUSB.println("routine");
	if(position_updated){
		alt_agl();
		update_wind();
		//		SerialUSB.println("agl");

	}
	
	update_tracklog();


	g_meter = pow(  ((float)ax*ax + (float)ay*ay + (float)az*az)/268.4e6 , 0.5) ;
	
	if(present_devices & SI7021_PRESENT){
		if(read_si7021())
		request_si7021();
	}
	yaw = Madgwick_filter.getYaw();
	pitch = Madgwick_filter.getPitch();
	roll = Madgwick_filter.getRoll();

	
}


void update_tracklog(){
	

	if (present_devices & SD_PRESENT)	{
		if((statVar.ena_vector & ENA_TRACKLOG) || tracklog_stat == TRACKLOG_FILE_OPEN){
			if (tracklog_stat == TRACKLOG_FILE_CLOSED){
				if(fix.valid.time){
					//SerialUSB.println("opening");
					
					char cesta_char[40];
					
					
					
					
					String cesta = ("/Tracks/");
					cesta.concat(String(rtc.getYear()));
					cesta.concat("/");
					cesta.concat(String(rtc.getMonth()));
					cesta.concat("/");
					cesta.concat(String(rtc.getDay()));
					
					cesta.toCharArray(cesta_char, 40);
					
					if(SD.exists(cesta_char));
					//SerialUSB.println("existuje");
					else{
						//SerialUSB.println(cesta_char);
						SD.mkdir(cesta_char);
						//SerialUSB.println("vytvoreno");
					}
					
					cesta.concat("/");
					
					int path_sec = rtc.getSeconds();
					int path_hours = rtc.getHours();
					int path_min = rtc.getMinutes();
					
					if(path_hours < 10) cesta.concat("0");
					cesta.concat(String(path_hours));
					if(path_min < 10) cesta.concat("0");
					cesta.concat(String(path_min));
					if(path_sec < 10) cesta.concat("0");
					cesta.concat(String(path_sec));
					cesta.concat(String(".gpx"));
					cesta.toCharArray(cesta_char, 40);
					
					tracklog = SD.open(cesta_char, FILE_WRITE);
					
					tracklog.println("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n");
					
					tracklog.println("<gpx version=\"1.1\" creator=\"mojehustevario\">");
					tracklog.println("  <trk>");
					tracklog.println("    <trkseg>");
					tracklog_stat = TRACKLOG_FILE_OPEN;
					
					
				}
			}
			if (tracklog && position_updated){
				//update_tracklog = 0;
				//	position_updated = 0;
				//	SerialUSB.println("logging");
				if(fix.valid.location){
					tracklog.print("      <trkpt lat=\"");
					tracklog.print(fix.latitude(), 6);
					tracklog.print("\" lon=\"");
					tracklog.print(fix.longitude(),5);
					tracklog.println("\">");
					
					tracklog.print("        <ele>");
					tracklog.print(fix.altitude(), 1);
					tracklog.println("</ele>");
					
					tracklog.print("        <note>,");
					tracklog.print(enviromental_data.temperature, 1);
					tracklog.print(",");
					tracklog.print(enviromental_data.humidity, 1);
					tracklog.print(",");
					tracklog.print(enviromental_data.pressure, 1);
					tracklog.print(",");
					tracklog.print(alt_filter/100, 1);
					tracklog.print(",");
					tracklog.print(vario_filter, 1);
					tracklog.print(",");
					tracklog.print(g_meter, 3);
					tracklog.print(",");
					tracklog.print(fix.valid.speed);
					tracklog.print(",");
					tracklog.print(fix.valid.heading);
					tracklog.print(",");
					tracklog.print(fix.speed_kph());
					tracklog.print(",");
					tracklog.print(fix.heading());
					tracklog.print(",");
					tracklog.print(wind_speed_mps);
					tracklog.print(",");
					tracklog.print(wind_direction);
					tracklog.print(",");
					tracklog.print(movement_circle_fit.a);
					tracklog.print(",");
					tracklog.print(movement_circle_fit.b);
					tracklog.print(",");
					tracklog.print(movement_circle_fit.r);
					tracklog.print(",");
					
					tracklog.print(ax);
					tracklog.print(",");
					tracklog.print(ay);
					tracklog.print(",");
					tracklog.print(az);
					tracklog.print(",");
					tracklog.print(gx);
					tracklog.print(",");
					tracklog.print(gy);
					tracklog.print(",");
					tracklog.print(gz);
					tracklog.print(",");
					tracklog.print(mx);
					tracklog.print(",");
					tracklog.print(my);
					tracklog.print(",");
					tracklog.print(mz);
					tracklog.print(",");
					tracklog.print(mx_cor);
					tracklog.print(",");
					tracklog.print(my_cor);
					tracklog.print(",");
					tracklog.print(mz_cor);
					tracklog.print(",");
					
					tracklog.print(ax_avg);
					tracklog.print(",");
					tracklog.print(ay_avg);
					tracklog.print(",");
					tracklog.print(az_avg);
					tracklog.print(",");
					tracklog.print(gx_avg);
					tracklog.print(",");
					tracklog.print(gy_avg);
					tracklog.print(",");
					tracklog.print(gz_avg);
					tracklog.print(",");
					
					tracklog.print(yaw);
					tracklog.print(",");
					tracklog.print(pitch);
					tracklog.print(",");
					tracklog.print(roll);
					tracklog.print(",");
					tracklog.print(ground_level);
					tracklog.print(",");
					tracklog.print(ground_level_interpol);
					tracklog.print(",");
					tracklog.print(fix.valid.date);
					tracklog.print(",");
					tracklog.print(fix.valid.time);
					tracklog.print(",");
					
					
					
					
					
					tracklog.println("</note>");
					
					
					
					
					
					/*
					
					tracklog.print("        <dataX>,");
					for(int i = 0; i < WIND_RBUFF_SIZE; i++){
					tracklog.print(movement_vector_data.X[i]);
					tracklog.print(",");
					}
					tracklog.println("</dataX>");
					
					tracklog.print("        <dataY>,");
					for(int i = 0; i < WIND_RBUFF_SIZE; i++){
					tracklog.print(movement_vector_data.Y[i]);
					tracklog.print(",");
					}
					tracklog.println("</dataY>");
					*/
					
					tracklog.print("        <time>20");
					
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
				
				if(tracklog_stat == TRACKLOG_FILE_OPEN && !(statVar.ena_vector & ENA_TRACKLOG)){
					tracklog.println("    </trkseg>");
					tracklog.println("  </trk>");
					tracklog.println("</gpx>");
					tracklog.close();
					tracklog_stat = TRACKLOG_FILE_CLOSED;
					//	SerialUSB.println("closing file");
				}
			}
			
			
			
		}
	}
	
}

void update_wind(){
	if(fix.valid.speed && fix.valid.heading){
		//update_wind = 0;
		if(fix.dateTime.seconds%2 == 0){
			float x,y;
			x = fix.speed_kph() * KPH2MPS * cos(fix.heading() * DEG2RAD);
			y = fix.speed_kph() * KPH2MPS * sin(fix.heading() * DEG2RAD);
			movement_vector_data.X[movement_vector_data.write_index % WIND_RBUFF_SIZE] = x;
			movement_vector_data.Y[movement_vector_data.write_index % WIND_RBUFF_SIZE] = y;
			movement_vector_data.write_index++;
			
			movement_circle_fit = CircleFitByTaubin(movement_vector_data);
			wind_speed_mps = sqrt(movement_circle_fit.a*movement_circle_fit.a + movement_circle_fit.b*movement_circle_fit.b);
			wind_direction = atan2(movement_circle_fit.a, movement_circle_fit.b)*RAD2DEG;
			if(wind_direction < 0)
			wind_direction += 360;
		}
		
	}
	

}

uint16_t getAGLfromFile(int row, int col){
	HeightData.seek((row*1201+col)*2);
	int lsbs = HeightData.read();
	int msbs = HeightData.read();
	return (lsbs << 8)+msbs;
}

void alt_agl(){
	


		
		float fix_lat = fix.latitude();
		float fix_lon = fix.longitude();
		

		
		
		
		unsigned long time_a = micros();
		unsigned long time_b;
		
		if(fix.valid.location && (present_devices & SD_PRESENT)){
			int int_lat = (int)fix_lat;
			int int_lon = (int)fix_lon;

			
			int x1y1, x2y1, x1y2, x2y2, x1, x2, y1, y2;
			float fractx, fracty, x1inter, x2inter, interpolation;
			
			
			x1 = floor(((float)fix_lon - int_lon)*1200.0f);
			x2 = ceil(((float)fix_lon - int_lon)*1200.0f);
			y1 = floor(1200.0f-((float)fix_lat - int_lat)*1200.0f);
			y2 = ceil(1200.0f-((float)fix_lat - int_lat)*1200.0f);
			

			
			/*
			x1 = ((float)fix.longitude() - int_lon)*1200.0f;
			x2 = x1+1;
			y1 = 1201.0f-((float)fix.latitude() - int_lat)*1200.0f;
			y2 = y1+1;
			*/
			
			fractx = (((float)fix_lon - int_lon)*1200.0f) - x1;
			fracty = (1200.0f-((float)fix_lat - int_lat)*1200.0f) - y1;
			


			
			
			float temp_lat  = round(1201.0f-((float)fix_lat - int_lat)*1200.0f);
			float temp_lon = round(((float)fix_lon - int_lon)*1200.0f);
			
			
			int row = (int)temp_lat;
			int col = (int)temp_lon;
			

			
			char cesta_char2[20];
			


			
			//open corresponding HGT file if moved to different coordinates
			if((int_lat != lat_last) || (int_lon != lon_last) || !HeightData){
				SerialUSB.println("opening");
				time_a = micros();


				if(HeightData)	HeightData.close();
				if((int_lat != lat_last) || (int_lon != lon_last)) filenotpresent == 0;
				if(filenotpresent == 0){
					lat_last = int_lat;
					lon_last = int_lon;
					
					
					String cesta = ("/HGT/");
					if(fix.latitude() < 0){
						cesta.concat("S");
						int_lat--;
					}
					else{
						cesta.concat("N");
					}
					if(int_lat < 10 && int_lat > -10){
						cesta.concat("0");
					}
					cesta.concat(String(abs(int_lat)));

					if(fix.longitude() < 0){
						cesta.concat("W");
						int_lon--;
					}
					else{
						cesta.concat("E");
					}
					if(int_lon < 10 && int_lon > -10){
						cesta.concat("0");
					}
					if(int_lon < 100 && int_lon > -100){
						cesta.concat("0");
					}
					cesta.concat(String(abs(int_lon)));
					cesta.concat(".HGT");
					cesta.toCharArray(cesta_char2, 20);
					
					HeightData = SD.open(cesta_char2, FILE_READ);
					time_b = micros();
					if(!HeightData) filenotpresent = 1;
				}
				
			}

			
			if(HeightData){
				ground_level = getAGLfromFile(row, col);
				x1y1 =  getAGLfromFile(y1, x1);
				x2y1 =  getAGLfromFile(y1, x2);
				x1y2 =  getAGLfromFile(y2, x1);
				x2y2 =  getAGLfromFile(y2, x2);

				x1inter = (1-fractx) * x1y1 + fractx*x2y1;
				x2inter = (1-fractx) * x1y2 + fractx*x2y2;
				interpolation = (1-fracty)* x1inter	+ fracty* x2inter;

				ground_level_interpol = interpolation;
				
				

			}
			else{
				ground_level = 0;
				SerialUSB.println("not open");
			}
			/*
			SerialUSB.print(x1);
			SerialUSB.print(",");
			SerialUSB.print(x2);
			SerialUSB.print(",");
			SerialUSB.print(y1);
			SerialUSB.print(",");
			SerialUSB.print(y2);
			SerialUSB.print(",");
			
			SerialUSB.print(fractx, 3);
			SerialUSB.print(",");
			SerialUSB.print(fracty, 3);
			SerialUSB.print(",");
			SerialUSB.print(ground_level);
			SerialUSB.print(",");
			SerialUSB.print(x1y1);
			SerialUSB.print(",");
			SerialUSB.print(x2y1);
			SerialUSB.print(",");
			SerialUSB.print(x1y2);
			SerialUSB.print(",");
			SerialUSB.print(x2y2);
			SerialUSB.print(",");
			SerialUSB.print(interpolation);
			SerialUSB.println(",");
			*/
		}

	
	//SerialUSB.println(time_b-time_a);
	
}





float calcDistanceFromCoordinates(double latHome, double lonHome, double latDest, double lonDest) {
	double pi = 3.141592653589793;
	int R = 6371; //Radius of the Earth
	latHome = (pi/180)*(latHome);
	latDest = (pi/180)*(latDest);
	double differenceLon = (pi/180)*(lonDest - lonHome);
	double differenceLat = (latDest - latHome);
	double a = sin(differenceLat/2) * sin(differenceLat/2) +
	cos(latHome) * cos(latDest) *
	sin(differenceLon/2) * sin(differenceLon/2);
	double c = 2 * atan2(sqrt(a), sqrt(1-a));
	double distance = R * c;
	//	SerialUSB.println(distance);

	return distance;
}

float calcHeadingFromCoordinates(double latHome, double lonHome, double latDest, double lonDest) {

	double pi = 3.141592653589793;
	int R = 6371; //Radius of the Earth
	latHome = (pi/180)*(latHome);
	latDest = (pi/180)*(latDest);
	double differenceLon = (pi/180)*(lonDest - lonHome);
	double differenceLat = (latDest - latHome);
	float heading = atan2(sin(differenceLon) * cos(latDest), cos(latHome)*sin(latDest)-sin(latHome)*cos(latDest)*cos(differenceLon));
	heading *=180/pi;
	if(heading<0) heading +=360;
	//	SerialUSB.println(heading);
	return heading;

}