/*
* IMU.cpp
*
* Created: 26.2.2019 20:53:55
*  Author: Jena
*/

#include "MEMS.h"



#define BMI160_SPI_READ_BIT         7

double C0, C1, C2, C3, C4, C5;


struct bmi160_dev imu;
struct bmi160_fifo_frame fifo_frame;

struct bmm150_dev bmm150;





int8_t write_sensor(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len){
	digitalWrite(dev_addr, 0);
	//SPI_IRQ.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
	
	SPI_IRQ.transfer(reg_addr);
	SPI_IRQ.transfer(data, len);
	//sercom1.transferDataSPI(data, len);
	digitalWrite(dev_addr, 1);
	//  	SerialUSB.println("sensor write");
	return 0;
}

int8_t read_sensor(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len){
	digitalWrite(dev_addr, 0);
	//SPI_IRQ.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE0));
	
	SPI_IRQ.transfer(reg_addr | (1<<BMI160_SPI_READ_BIT ));
	//SPI_IRQ.transfer(data, len);
	sercom1.transferDataSPI(data, len);
	digitalWrite(dev_addr, 1);
	//SerialUSB.println("sensor read");
	
	return 0;
}


/*wrapper function to match the signature of bmm150.read */
int8_t user_aux_read(uint8_t id, uint8_t reg_addr, uint8_t *aux_data, uint16_t len)
{
	int8_t rslt;
	
	/* Discarding the parameter id as it is redundant*/
	rslt = bmi160_aux_read(reg_addr, aux_data, len, &imu);

	return rslt;
}

/*wrapper function to match the signature of bmm150.write */
int8_t user_aux_write(uint8_t id, uint8_t reg_addr, uint8_t *aux_data, uint16_t len)
{
	int8_t rslt;
	
	/* Discarding the parameter id as it is redundant */
	rslt = bmi160_aux_write(reg_addr, aux_data, len, &imu);

	return rslt;
}

void delayWrapper(uint32_t period){
	delayMicroseconds(period*1000);
	
}



int8_t IMU_init(){
	SPI_IRQ.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE0));
	imu.id = IMU_CS;
	imu.interface = BMI160_SPI_INTF;
	imu.read = read_sensor;
	imu.write = write_sensor;
	imu.delay_ms = delayWrapper;


	int8_t rslt = BMI160_OK;
	rslt = bmi160_init(&imu);

	/* Select the Output data rate, range of accelerometer sensor */
	imu.accel_cfg.odr = BMI160_ACCEL_ODR_100HZ;
	imu.accel_cfg.range = BMI160_ACCEL_RANGE_4G;
	imu.accel_cfg.bw = BMI160_ACCEL_BW_NORMAL_AVG4;

	/* Select the power mode of accelerometer sensor */
	imu.accel_cfg.power = BMI160_ACCEL_NORMAL_MODE;


	/* Select the Output data rate, range of Gyroscope sensor */
	imu.gyro_cfg.odr = BMI160_GYRO_ODR_100HZ;
	imu.gyro_cfg.range = BMI160_GYRO_RANGE_250_DPS;
	imu.gyro_cfg.bw = BMI160_GYRO_BW_NORMAL_MODE;

	/* Select the power mode of Gyroscope sensor */
	imu.gyro_cfg.power = BMI160_GYRO_NORMAL_MODE;

	/* Set the sensor configuration */
	rslt |= bmi160_set_sens_conf(&imu);
	MAG_init();

	fifo_frame.length = 0;
	imu.fifo = &fifo_frame;
	rslt |= bmi160_set_fifo_config(0xff,	BMI160_DISABLE, &imu);
	rslt |= bmi160_set_fifo_config( BMI160_FIFO_AUX |BMI160_FIFO_ACCEL | BMI160_FIFO_GYRO | BMI160_FIFO_HEADER,	BMI160_ENABLE, &imu);
	SPI_IRQ.endTransaction();
	return rslt;



}


int8_t MAG_init(){
	SPI_IRQ.beginTransaction(SPISettings(5000000, MSBFIRST, SPI_MODE0));
	imu.aux_cfg.aux_sensor_enable = 1; // auxiliary sensor enable
	imu.aux_cfg.aux_i2c_addr = BMI160_AUX_BMM150_I2C_ADDR; // auxiliary sensor address
	imu.aux_cfg.manual_enable = 1; // setup mode enable
	imu.aux_cfg.aux_rd_burst_len = 2;// burst read of 2 byte
	
	/* Configure the BMM150 device structure by
	mapping user_aux_read and user_aux_write */
	bmm150.read = user_aux_read;
	bmm150.write = user_aux_write;
	bmm150.dev_id = BMM150_DEFAULT_I2C_ADDRESS;
	/* Ensure that sensor.aux_cfg.aux_i2c_addr = bmm150.id
	for proper sensor operation */
	bmm150.delay_ms = delayWrapper;
	bmm150.intf = BMM150_I2C_INTF;

	/* Initialize the auxiliary sensor interface */
	int8_t rslt = bmi160_aux_init(&imu);

	/* Auxiliary sensor is enabled and can be accessed from this point */

	/* Configure the desired settings in auxiliary BMM150 sensor
	* using the bmm150 APIs */

	/* Initialising the bmm150 sensor */
	rslt |= bmm150_init(&bmm150);

	/* Set the power mode and preset mode to enable Mag data sampling */
	bmm150.settings.pwr_mode = BMM150_FORCED_MODE;
	rslt |= bmm150_set_op_mode(&bmm150);

	bmm150.settings.preset_mode= BMM150_PRESETMODE_LOWPOWER;
	rslt |= bmm150_set_presetmode(&bmm150);
	
	
	
	
	
	
	/* Initialize the Auxiliary BMM150 following the above code
	* until setting the power mode (Set the power mode as forced mode)
	* and preset mode */

	/* In BMM150 Mag data starts from register address 0x42 */
	uint8_t aux_addr = 0x42;
	/* Buffer to store the Mag data from 0x42 to 0x48 */
	
	/* Configure the Auxiliary sensor either in auto/manual modes and set the
	polling frequency for the Auxiliary interface */
	imu.aux_cfg.aux_odr = BMI160_AUX_ODR_100HZ ; /* Represents polling rate in 100 Hz*/
	rslt |= bmi160_config_aux_mode(&imu);
	
	uint8_t a = 0;
	bmi160_aux_write(BMI160_AUX_IF_1_ADDR, &a, 1, &imu);
	
	
	/* Set the auxiliary sensor to auto mode */
	rslt |= bmi160_set_aux_auto_mode(&aux_addr, &imu);

	SPI_IRQ.endTransaction();
	
	
	
}

void read_mag(){
	//MAG_init();
	uint8_t dummy[20];
	bmi160_get_regs(0x04, dummy, 20, &imu);
	
	uint8_t mag_data[8] = {0};
	uint8_t index;
	
	int8_t rslt = bmi160_read_aux_data_auto_mode(mag_data, &imu);

	SerialUSB.println("\n RAW DATA ");
	for(index = 0 ; index < 8 ; index++)
	{
		SerialUSB.println( mag_data[index]);
	}
	
	/* Compensating the raw mag data available from the BMM150 API */
	rslt |= bmm150_aux_mag_data(mag_data, &bmm150);
	
	SerialUSB.println("\n COMPENSATED DATA ");
	SerialUSB.print(bmm150.data.x);
	SerialUSB.println(",");
	SerialUSB.print(bmm150.data.y);
	SerialUSB.println(",");
	SerialUSB.println(bmm150.data.z);
	
	
}



void IMU_read(){
	int8_t rslt = BMI160_OK;
	struct bmi160_sensor_data accel;
	struct bmi160_sensor_data gyro;

	/* To read only Accel data */
	rslt = bmi160_get_sensor_data(BMI160_GYRO_SEL, &accel, &gyro, &imu);
	
	//rslt = bmi160_get_fifo_data(dev);
	
	


	
	SerialUSB.print(gyro.x);
	SerialUSB.print(",");
	SerialUSB.print(gyro.y);
	SerialUSB.print(",");
	SerialUSB.print(gyro.z);
	SerialUSB.println(",");
	
}


int IMU_FifoBytesToRead(){
	int bytes_to_read = 0;
	uint8_t data[2];

	//bmi160_get_regs(BMI160_FIFO_LENGTH_ADDR, data, 2, &imu);
	read_sensor(imu.id, BMI160_FIFO_LENGTH_ADDR, data, 2);
	data[1] = data[1] & BMI160_FIFO_BYTE_COUNTER_MASK;

	/* Available data in FIFO is stored in bytes_to_read*/
	bytes_to_read = (((uint16_t)data[1] << 8) | ((uint16_t)data[0]));
	return bytes_to_read;
}

void print_mag(){
	SerialUSB.print(mx_cor);
	SerialUSB.print(",");
	SerialUSB.print(my_cor);
	SerialUSB.print(",");
	SerialUSB.println(mz_cor);

}

int IMU_ReadFrameFromFifo(){
	

	uint8_t fifodata[FIFO_FRAME_SIZE];
	read_sensor(imu.id, BMI160_FIFO_DATA_ADDR, fifodata, FIFO_FRAME_SIZE);
	if(fifodata[0] != 128){
		
		bmm150_aux_mag_data(&fifodata[1], &bmm150);
		mx = bmm150.data.x;
		my = bmm150.data.y;
		mz = bmm150.data.z;
		


		gx = fifodata[9] | ((int8_t)fifodata[10] << 8);
		gy = fifodata[11] | ((int8_t)fifodata[12] << 8);
		gz = fifodata[13] | ((int8_t)fifodata[14] << 8);

		ax = fifodata[15] | ((int8_t)fifodata[16] << 8);
		ay = fifodata[17] | ((int8_t)fifodata[18] << 8);
		az = fifodata[19] | ((int8_t)fifodata[20] << 8);

	}

	
	
}



int read_si7021(){
	
	// Wire.beginTransmission(SI7021_ADDRESS);
	//Wire.write(SI7021_MEASURE_RH);
	// Wire.endTransmission();
	// int byte = 0;
	// delay(15);
	
	//  while(!byte){
	//    byte = Wire.requestFrom(SI7021_ADDRESS, 2);
	//  }
	if(Wire.requestFrom(SI7021_ADDRESS, 2)){
		int rh = 0;
		rh = Wire.read()<<8;
		rh += Wire.read();
		float humidity = (125.0f*rh/65536.0f)-6.0f;
		if(humidity <= 0) humidity = 0;
		else if(humidity >= 100) humidity = 100;
		enviromental_data.humidity = humidity;
		//  SerialUSB.print((125.0*rh/65536.0)-6.0);
		//  SerialUSB.print(',');
		
		
		
		Wire.beginTransmission(SI7021_ADDRESS);
		Wire.write(SI7021_READ_TEMP_FROM_RH);
		Wire.endTransmission();

		if(Wire.requestFrom(SI7021_ADDRESS, 2)){

			rh = 0;
			rh = Wire.read()<<8;
			rh += Wire.read();
			float temperature = (175.72f*rh/65536.0f)-46.85f;
			enviromental_data.temperature = temperature*100;
			return 1;
			
		}
		
	}
	return 0;
}

int request_si7021(){
	Wire.beginTransmission(SI7021_ADDRESS);
	Wire.write(SI7021_MEASURE_RH);
	return Wire.endTransmission();
}


int lps33_write(char addr, char data) {
	digitalWrite(BARO_CS, 0);
	SPI_IRQ.transfer(addr & 0b01111111);
	SPI_IRQ.transfer(data);
	digitalWrite(BARO_CS, 1);
	return 1;
}

int lps33_read(char addr) {
	digitalWrite(BARO_CS, 0);
	SPI_IRQ.transfer(addr | 0b10000000);
	
	int data = SPI_IRQ.transfer(0xff);
	digitalWrite(BARO_CS, 1);
	return data;
}


void lps33_init(){
	lps33_write(0x11, 0b00011100);
	while((lps33_read(0x11)&0x04)){
		delay(1);
	}
	lps33_write(LPS_CTRL_REG1, 0b01010000);
}

int lps33_test(){
	if(lps33_read(0x0f) == LPS_ID)
	return 1;
	else
	return 0;
}

void baro_readPressure(){
	uint8_t data[4] = {0x28|0x80, 0xff, 0xff, 0xff};
	digitalWrite(BARO_CS, 0);
	
	sercom1.transferDataSPI(data, 4);
	/*
	SPI_IRQ.transfer(0x28 | 0b10000000);
	
	int data0 = SPI_IRQ.transfer(0xff);
	
	int data1 = SPI_IRQ.transfer(0xff);
	int data2 = SPI_IRQ.transfer(0xff);
	*/
	digitalWrite(BARO_CS, 1);
	uint32_t temp =  (int)data[1] + ((int)data[2]<<8) + ((int)data[3]<<16);
	//enviromental_data.pressure = temp;
	//enviromental_data.pressure = (float)temp/0.4096f;
	enviromental_data.pressure = (float)temp*2.44141f;
	
}


void setSeaPressureFromAltitude(int altitude, unsigned int pressure){	
	statVar.Psea = (double)pressure/100.0 *  pow((1.0 - 0.0065 * altitude / (enviromental_data.temperature / 100 + 0.0065 * altitude + 273.15)), -5.257)  ;
}

void setSeaPressure(int pressure) {
	statVar.Psea = pressure  ;
}




float getAltitude(){

	
	
	float diff = statVar.Psea*100.0f/(enviromental_data.pressure) - 1.3f;
	float diff2 = diff * diff;
	
	//some crazy taylor series magic, bcs aint nobody has time for pow()
	float power = (1.05099483835522f
	+ 0.153261726531934f * diff
	+ -0.0477720927877293f * diff2
	+ 0.0221763811870283f * diff2 * diff
	+ -0.0119855962404455f * diff2 * diff2);

	
	return ((power -1.0f)* (enviromental_data.temperature/100.0f + 273.15f) / 0.0065f);
	
	
	
}