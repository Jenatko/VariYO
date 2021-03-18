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

#include "CircleFit/CircleFitByTaubin.h"

#include <MadgwickAHRS/MadgwickAHRS.h>

#include "Gauge.h"


NMEAGPS  gps;
gps_fix  fix;

#include "SdFat.h"

extern SdFat SD;

SdFile tracklog;
SdFile HeightData;
SdFile loggerFile;




int sec_old = 0;
int lat_last, lon_last, filenotpresent;
int filesize;

// Some HGT files contain 1201 x 1201 points (3 arc/90m resolution)
#define HGT_DATA_WIDTH_3	1201ul

// Some HGT files contain 3601 x 3601 points (1 arc/30m resolution)
#define HGT_DATA_WIDTH_1	3601ul

// Some HGT files contain 3601 x 1801 points (1 arc/30m resolution)
#define HGT_DATA_WIDTH_1_HALF	1801ul

// number of decimal places in int representation of fix
#define GPS_COORD_MUL 10000000



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

void routine(int OnlyReadGPS){


	//updating GPS
	if(statVar.ena_vector & ENA_GPS){
		SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
		digitalWrite(GPS_CS, 0);
		byte fn;
		do {
			fn = SPI.transfer( 0xFF );
			//		if(fn > 127 && fn != 0xff)

			//		while(digitalRead(BUTTON_LEFT))
			//		SerialUSB.write(fn);   //uncomment for sending GPS data over Serial (to work with u-center)
		} while (gps.handle(fn) != NMEAGPS::DECODE_CHR_INVALID || fn != 0xff );
		digitalWrite(GPS_CS, 1);
		if(gps.available()){
			fix = gps.read();
			
			if(fix.valid.time){
				rtc.setTime(fix.dateTime.hours, fix.dateTime.minutes, fix.dateTime.seconds);
			}
			if(fix.valid.date){
				rtc.setDate(fix.dateTime.date, fix.dateTime.month, fix.dateTime.year);
			}
			
			if(fix.valid.location)
			updateGauge(&statVar.speedGauge, fix.speed_kph());
			else
			updateGauge(&statVar.speedGauge, NAN);
			
			if(fix.valid.heading)
			updateGauge(&statVar.headingGauge, fix.heading());
			else
			updateGauge(&statVar.headingGauge, NAN);
			
			
			

			position_updated = 1;
			
			
			//debug to determine ranges for gyro and accelerometers
			axmax = 0;
			axmin = 0;
			aymax = 0;
			aymin = 0;
			azmax = 0;
			azmin = 0;
			gxmax = 0;
			gxmin = 0;
			gymax = 0;
			gymin = 0;
			gzmax = 0;
			gzmin = 0;
			
		}
		else{
			position_updated = 0;
		}
		//procedure for capturing commands sent to GPS

		//SerialUSB.println("-----------------");
		//while(SerialUSB.available()) {      // If anything comes in Serial (USB),
		//SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
		//digitalWrite(GPS_CS, 0);
		//fn = SerialUSB.read();
		//gps.handle(SPI.transfer(fn));
		//SerialUSB.print(fn);
		//digitalWrite(GPS_CS, 1);
		//
		//}
		//SerialUSB.println("-----------------");
		//delay(50);

		
	}
	//update display if GPS is off
	//not needed anymore since display is refreshing as quickly as possible
	/*
	else{
	int sec = rtc.getSeconds();
	if( sec != sec_old){
	//	redraw = 1;
	sec_old = sec;
	
	}

	position_updated = 0;
	}
	*/

	if(OnlyReadGPS == 0){
		if(position_updated){
			alt_agl();
			update_wind();

		}
		
		update_tracklog();
		
		if(statVar.ena_vector & ENA_TRACKLOG){
			if(maxalt < alt_filter) maxalt = alt_filter;
			if(minalt > alt_filter) minalt = alt_filter;
			if(maxrise10s < vario_lowpassed_600samples) maxrise10s = vario_lowpassed_600samples;
			if(minsink10s > vario_lowpassed_600samples) minsink10s = vario_lowpassed_600samples;
			
			
		}


		g_meter = pow(  ((float)ax*ax + (float)ay*ay + (float)az*az)/268.4e6 , 0.5) ;
		
		if(present_devices & SI7021_PRESENT){
			if(read_si7021())
			request_si7021();
		}
		
		if(present_devices & MAX17055_PRESENT){
			battery_SOC = max17055.getSOC();
			battery_voltage = max17055.getAverageVoltage();
			
			if(battery_voltage < 3.4){
				powerOff(1, 0);
				
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
					
					tracklog.open(cesta_char, O_WRITE | O_CREAT);
					
					
					tracklog.println("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n");
					
					tracklog.println("<gpx version=\"1.1\" creator=\"mojehustevario\">");
					tracklog.println("        <note>,latitude,longitude,gps_altitude,temperature, humidity, pressure, alt_filter, vario, g_meter, valid_speed, valid_heading, speed, heading, wind speed, wind direction, circle fit a, circle fit b, circle fit r, yaw, pitch, roll, ground level, ground level interpol, ground_lvl_skydrop, valid date, valid time, axmax, axmin, aymax, aymin, azmax, azmin, gxmax, gxmin, gymax, gymin, gzmax, gzmin,x1y1,x2y1,x1y2,x2y2,alt11, alt12, alt21,alt22,fractx,fracty,lat_dr,lon_dr</note>");
					tracklog.println("  <trk>");
					tracklog.println("    <trkseg>");

					tracklog_stat = TRACKLOG_FILE_OPEN;
					
					
				}
			}
			if (tracklog.isOpen() && position_updated){
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
					tracklog.print(fix.latitude(), 6);
					tracklog.print(",");
					tracklog.print(fix.longitude(),6);
					tracklog.print(",");
					tracklog.print(fix.altitude(), 1);
					tracklog.print(",");
					tracklog.print(enviromental_data.temperature*0.01f, 1);
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
					tracklog.print(ground_level2);
					tracklog.print(",");
					tracklog.print(fix.valid.date);
					tracklog.print(",");
					tracklog.print(fix.valid.time);
					tracklog.print(",");
					tracklog.print(axmax);
					tracklog.print(",");
					tracklog.print(axmin);
					tracklog.print(",");
					tracklog.print(aymax);
					tracklog.print(",");
					tracklog.print(aymin);
					tracklog.print(",");
					tracklog.print(azmax);
					tracklog.print(",");
					tracklog.print(azmin);
					tracklog.print(",");
					
					tracklog.print(gxmax);
					tracklog.print(",");
					tracklog.print(gxmin);
					tracklog.print(",");
					tracklog.print(gymax);
					tracklog.print(",");
					tracklog.print(gymin);
					tracklog.print(",");
					tracklog.print(gzmax);
					tracklog.print(",");
					tracklog.print(gzmin);
					tracklog.print(",");
					tracklog.print(x1y1);
					tracklog.print(",");
					tracklog.print(x2y1);
					tracklog.print(",");
					tracklog.print(x1y2);
					tracklog.print(",");
					tracklog.print(x2y2);
					tracklog.print(",");
					tracklog.print(alt11);
					tracklog.print(",");
					tracklog.print(alt12);
					tracklog.print(",");
					tracklog.print( alt21);
					tracklog.print(",");
					tracklog.print( alt22);
					tracklog.print(",");
					tracklog.print( fractx);
					tracklog.print(",");
					tracklog.print( fracty);
					tracklog.print(",");
					tracklog.print( lat_dr);
					tracklog.print(",");
					tracklog.print( lon_dr);
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
					if(fix.dateTime.month < 10) tracklog.print("0");
					tracklog.print(fix.dateTime.month);
					tracklog.print("-");
					if(fix.dateTime.date < 10) tracklog.print("0");
					tracklog.print(fix.dateTime.date);
					tracklog.print("T");
					if(fix.dateTime.hours < 10) tracklog.print("0");
					tracklog.print(fix.dateTime.hours);
					tracklog.print(":");
					if(fix.dateTime.minutes < 10) tracklog.print("0");
					tracklog.print(fix.dateTime.minutes);
					tracklog.print(":");
					if(fix.dateTime.seconds < 10) tracklog.print("0");
					tracklog.print(fix.dateTime.seconds);
					
					tracklog.println("Z</time>");
					tracklog.println("      </trkpt>");
				}
				
				if(tracklog_stat == TRACKLOG_FILE_OPEN && !(statVar.ena_vector & ENA_TRACKLOG)){
					tracklog.println("    </trkseg>");
					tracklog.println("  </trk>");
					tracklog.println("</gpx>");
					tracklog.sync();
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
	HeightData.seekSet(0);
	HeightData.seekCur((row*1201+col)*2);
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
		int int_lat = fix.latitudeL()/GPS_COORD_MUL;
		int int_lon = fix.longitudeL()/GPS_COORD_MUL;

		
		//	int x1y1, x2y1, x1y2, x2y2;
		//int alt11, alt12, alt21, alt22;
		int x1, x2, y1, y2;

		//float fractx, fracty, x1inter, x2inter, interpolation;
		
		
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
		

		
		
		


		
		//open corresponding HGT file if moved to different coordinates
		if((int_lat != lat_last) || (int_lon != lon_last) || !HeightData.isOpen()){
			time_a = micros();


			if(HeightData.isOpen())	HeightData.close();
			if((int_lat != lat_last) || (int_lon != lon_last)) filenotpresent == 0;
			if(filenotpresent == 0){
				char cesta_char2[20];
				lat_last = int_lat;
				lon_last = int_lon;
				
				
				String cesta = ("/HGT/");
				if(fix.latitudeL() < 0){
					cesta.concat("S");
					int_lat++;
				}
				else{
					cesta.concat("N");
				}
				if(int_lat < 10 && int_lat > -10){
					cesta.concat("0");
				}
				cesta.concat(String(abs(int_lat)));

				if(fix.longitudeL() < 0){
					cesta.concat("W");
					int_lon++;
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
				
				HeightData.open(cesta_char2, O_READ);
				time_b = micros();
				if(!HeightData.isOpen()) filenotpresent = 1;
				filesize = HeightData.fileSize();
			}
			
		}

		
		if(HeightData.isOpen()){
			
			ground_level = getAGLfromFile(row, col);
			x1y1 =  getAGLfromFile(y1, x1);
			x2y1 =  getAGLfromFile(y1, x2);
			x1y2 =  getAGLfromFile(y2, x1);
			x2y2 =  getAGLfromFile(y2, x2);

			x1inter = (1-fractx) * x1y1 + fractx*x2y1;
			x2inter = (1-fractx) * x1y2 + fractx*x2y2;
			interpolation = (1-fracty)* x1inter	+ fracty* x2inter;

			ground_level_interpol = interpolation;
			
			
			
			//skydrop algorithm
			
			int32_t lon = fix.longitudeL();
			int32_t lat = fix.latitudeL();
			uint16_t num_points_x;
			uint16_t num_points_y;
			int16_t alt;
			
			if (lon < 0)
			{
				// we do not care above degree, only minutes are important
				// reverse the value, because file goes in opposite direction.
				lon = (GPS_COORD_MUL - 1) + (lon % GPS_COORD_MUL);   // lon is negative!
			}
			if (lat < 0)
			{
				// we do not care above degree, only minutes are important
				// reverse the value, because file goes in opposite direction.
				lat = (GPS_COORD_MUL - 1) + (lat % GPS_COORD_MUL);   // lat is negative!
			}
			switch (filesize)
			{
				case HGT_DATA_WIDTH_3 * HGT_DATA_WIDTH_3 * 2:
				num_points_x = num_points_y = HGT_DATA_WIDTH_3;
				break;
				case HGT_DATA_WIDTH_1 * HGT_DATA_WIDTH_1 * 2:
				num_points_x = num_points_y = HGT_DATA_WIDTH_1;
				break;
				case HGT_DATA_WIDTH_1 * HGT_DATA_WIDTH_1_HALF * 2:
				num_points_x = HGT_DATA_WIDTH_1_HALF;
				num_points_y = HGT_DATA_WIDTH_1;
				break;
				default:
				ground_level = 0xffff;
			}

			// "-2" is, because a file has a overlap of 1 point to the next file.
			uint32_t coord_div_x = GPS_COORD_MUL / (num_points_x - 1);
			uint32_t coord_div_y = GPS_COORD_MUL / (num_points_y - 1);
			uint16_t y = (lat % GPS_COORD_MUL) / coord_div_y;
			uint16_t x = (lon % GPS_COORD_MUL) / coord_div_x;

			uint16_t rd;
			uint8_t tmp[4];
			
			


			//seek to position
			uint32_t pos = ((uint32_t) x + num_points_x * (uint32_t) ((num_points_y - y) - 1)) * 2;
			
			HeightData.seekSet(0);
			HeightData.seekCur(pos);
			int lsbs = HeightData.read();
			int msbs = HeightData.read();
			alt11 = (lsbs << 8)+msbs;
			lsbs = HeightData.read();
			msbs = HeightData.read();
			alt21 = (lsbs << 8)+msbs;
			

			//seek to opposite position
			pos -= num_points_x * 2;

			
			HeightData.seekSet(0);
			HeightData.seekCur(pos);
			lsbs = HeightData.read();
			msbs = HeightData.read();
			alt12 = (lsbs << 8)+msbs;
			lsbs = HeightData.read();
			msbs = HeightData.read();
			alt22 = (lsbs << 8)+msbs;


			//get point displacement
			/*float*/ lat_dr = ((lat % GPS_COORD_MUL) % coord_div_y) / float(coord_div_y);
			/*float*/ lon_dr = ((lon % GPS_COORD_MUL) % coord_div_x) / float(coord_div_x);

			//compute height by using bilinear interpolation
			float alt1 = alt11 + float(alt12 - alt11) * lat_dr;
			float alt2 = alt21 + float(alt22 - alt21) * lat_dr;

			ground_level2 = alt1 + float(alt2 - alt1) * lon_dr;



		}
		else ground_level = 0xffff; //valid location but missing HGT file
		

	}
	
	else ground_level = 0xffff; //not valid location
	
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