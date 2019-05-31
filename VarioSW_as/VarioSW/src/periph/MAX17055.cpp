#include <MAX17055.h>
#include <Wire.h>
 

// Constructors 
MAX17055::MAX17055(void)


  
{
	//doesn't need anything here
}

MAX17055::MAX17055(uint16_t batteryCapacity)
{
	//calcuation based on AN6358 page 13 figure 1.3 for Capacity, but reversed to get the register value 
		writeReg16Bit(ModelCFG, 0);
	writeReg16Bit(DesignCap, batteryCapacity*2);
}
// Public Methods
void MAX17055::setCapacity(uint16_t batteryCapacity)
{
			writeReg16Bit(ModelCFG, 0x8000);
	//calcuation based on AN6358 page 13 figure 1.3 for Capacity, but reversed to get the register value 
	writeReg16Bit(DesignCap, batteryCapacity / capacity_multiplier_mAH);	
}	

float MAX17055::getCapacity()
{
   	// uint16_t capacity_raw = readReg16Bit(RepCap);
   	uint16_t capacity_raw = readReg16Bit(DesignCap);
	return (capacity_raw * capacity_multiplier_mAH);
}

	float MAX17055::getReportedCapacity()
{
	// uint16_t capacity_raw = readReg16Bit(RepCap);
	uint16_t capacity_raw = readReg16Bit(FullCapRep);
	return (capacity_raw * capacity_multiplier_mAH);
}	
	
	float MAX17055::getRemainingCapacity()
{
	// uint16_t capacity_raw = readReg16Bit(RepCap);
	uint16_t capacity_raw = readReg16Bit(RepCap);
	return (capacity_raw * capacity_multiplier_mAH);
}

void MAX17055::setResistSensor(float resistorValue)
{
	resistSensor = resistorValue;
	 capacity_multiplier_mAH = (5e-3)/resistSensor; //refer to row "Capacity"
	 current_multiplier_mV = (1.5625e-3)/resistSensor; //refer to row "Current"
	 voltage_multiplier_V = 7.8125e-5; //refer to row "Voltage"
	 time_multiplier_Hours = 5.625/3600.0; //Least Significant Bit= 5.625 seconds, 3600 converts it to Hours. refer to AN6358 pg 13 figure 1.3 in row "Time"
	 percentage_multiplier = 1.0/256.0; //refer to row "Percentage"
}

float MAX17055::getResistSensor()
{
	return resistSensor;
}

float MAX17055::getInstantaneousCurrent()
{
   	int16_t current_raw = readReg16Bit(Current);
	return current_raw * current_multiplier_mV;
}

float MAX17055::getAverageCurrent()
{
	int16_t current_raw = readReg16Bit(AvgCurrent);
	return current_raw * current_multiplier_mV;
}

float MAX17055::getInstantaneousVoltage()
{
   	uint16_t voltage_raw = readReg16Bit(VCell);
	return voltage_raw * voltage_multiplier_V;
}

float MAX17055::getAverageVoltage()
{
	uint16_t voltage_raw = readReg16Bit(AvgVCell);
	return voltage_raw * voltage_multiplier_V;
}

float MAX17055::getSOC()
{
   	uint16_t SOC_raw = readReg16Bit(RepSOC);
	return SOC_raw * percentage_multiplier;
}

float MAX17055::getTimeToEmpty()
{
	uint16_t TTE_raw = readReg16Bit(TimeToEmpty);
	return TTE_raw * time_multiplier_Hours;
}

float MAX17055::getTimeToFull()
{
	uint16_t TTE_raw = readReg16Bit(TimeToFull);
	return TTE_raw * time_multiplier_Hours;
}

float MAX17055::getTemp()
{
	int16_t TTE_raw = readReg16Bit(Temp);
	return TTE_raw/256.0;
}

float MAX17055::getVFSOC(){
   	uint16_t SOC_raw = readReg16Bit(VFSOC);
   	return SOC_raw * percentage_multiplier;
	
}

int MAX17055::checkFunct(){
writeReg16Bit(Rsense, 0xaabb);
int readvalue = readReg16Bit(Rsense);
if(readvalue == 0xaabb)
return 1;
else
return 0;
}



// Private Methods

void MAX17055::writeReg16Bit(uint8_t reg, uint16_t value)
{
  //Write order is LSB first, and then MSB. Refer to AN635 pg 35 figure 1.12.2.5
  Wire.beginTransmission(I2CAddress);
  Wire.write(reg);
  Wire.write( value       & 0xFF); // value low byte
  Wire.write((value >> 8) & 0xFF); // value high byte
  uint8_t last_status = Wire.endTransmission();
}

uint16_t MAX17055::readReg16Bit(uint8_t reg)
{
  uint16_t value = 0;  
  Wire.beginTransmission(I2CAddress); 
  Wire.write(reg);
  uint8_t last_status = Wire.endTransmission(false);
  
  Wire.requestFrom(I2CAddress, (uint8_t) 2); 
  value  = Wire.read();
  value |= (uint16_t)Wire.read() << 8;      // value low byte
  return value;
}

MAX17055 max17055;
