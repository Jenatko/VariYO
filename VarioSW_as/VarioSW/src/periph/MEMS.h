/*
 * IMU.h
 *
 * Created: 26.2.2019 21:40:57
 *  Author: Jena
 */ 


#ifndef IMU_H_
#define IMU_H_

#include "definitions.h"
#include "SPI_IRQ.h"
#include "bmi160_bosch.h"
#include "Variables.h"
#include "BMM150_bosch_driver/bmm150.h"

//#include "BME280_bosch_driver/bme280.h" // Honzo toto zmazal

#include "Wire.h"

#define LPS_CTRL_REG1 0x10
#define LPS_ID 0b10110001
#define BMX160_ID 0b11011000
#define BMI160_ID 0b11010001

#define SI7021_ADDRESS 0x40
#define SI7021_MEASURE_RH 0xF5
#define SI7021_MEAUSRE_TEMP 0xF3
#define SI7021_READ_TEMP_FROM_RH 0xE0

//taylor coefficients for altitude calculation
extern double C0, C1, C2, C3, C4, C5;

//functions for communicating with bosch sensors
int8_t write_sensor(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len);

int8_t read_sensor(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len);

//wrapper functions for aux. interface
int8_t user_aux_read(uint8_t id, uint8_t reg_addr, uint8_t *aux_data, uint16_t len);
int8_t user_aux_write(uint8_t id, uint8_t reg_addr, uint8_t *aux_data, uint16_t len);





int8_t IMU_init();

int8_t MAG_init();

void IMU_read();

int8_t BME_init_1ms();
int8_t BME_init_1s();
int8_t BME_init_forced();

//read temperature, humidity and pressure from BME sensor, store in global structure StatVar.enviromental_data
void BME_read();


void setSeaPressure(int pressure);

void setSeaPressureFromAltitude(int altitude, unsigned int pressure);

//calculate altitude from pressure stored in StatVar.enviromental_data
float getAltitude();

int IMU_FifoBytesToRead();

int IMU_ReadFrameFromFifo();

void read_mag();

void print_mag();


void baro_readPressure();

int lps33_test();

int lps33_write(char addr, char data);
int lps33_read(char addr);
void lps33_init();

int read_si7021();
int request_si7021();


#endif /* IMU_H_ */