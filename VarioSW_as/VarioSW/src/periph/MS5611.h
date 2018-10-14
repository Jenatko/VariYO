/*
  MS5611.h - Header file for the MS5611 Barometric Pressure & Temperature Sensor Arduino Library.

  Version: 1.0.0
  (c) 2014 Korneliusz Jarzebski
  www.jarzebski.pl

  This program is free software: you can redistribute it and/or modify
  it under the terms of the version 3 GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "../SPI_IRQ.h"
#include "../definitions.h"

#ifndef MS5611_h
#define MS5611_h


#include <Arduino.h>



#define MS5611_ADDRESS                (0x77)

#define MS5611_CMD_ADC_READ           (0x00)
#define MS5611_CMD_RESET              (0x1E)
#define MS5611_CMD_CONV_D1            (0x40)
#define MS5611_CMD_CONV_D2            (0x50)
#define MS5611_CMD_READ_PROM          (0xA2)



#include <SPI.h>



typedef enum
{
  MS5611_ULTRA_HIGH_RES   = 0x08,
  MS5611_HIGH_RES         = 0x06,
  MS5611_STANDARD         = 0x04,
  MS5611_LOW_POWER        = 0x02,
  MS5611_ULTRA_LOW_POWER  = 0x00
} ms5611_osr_t;

class MS5611
{
  public:

    bool begin(ms5611_osr_t osr = MS5611_HIGH_RES);
    uint32_t readRawTemperature(void);
    uint32_t readRawPressure(void);
    uint32_t getRawTemperature(void);
    uint32_t getRawPressure(void);
    double readTemperature();
    int32_t readPressure();
    void setOversampling(ms5611_osr_t osr);
    ms5611_osr_t getOversampling(void);
    void tempUpdate(int stage);
    void setSeaLevel(int altitude, unsigned int pressure);
    void putSeaLevel(int pressure);
    double readSeaPressure();
    double readAltitude(unsigned int pressure);
    long getProm(int index);
    void requestPressure();
    void requestTemperature();
    uint32_t getADCreg();
    uint32_t pressureCorrection(uint32_t Pressure);
    uint32_t updatePressure(void);

  private:

    unsigned int fc[6];
    int ct;
    int uosr;
    int TEMP2, TEMP, dT;
    int OFF2, SENS2;
    unsigned int D1, D2, P;

    int64_t OFF ;
    int64_t SENS;

    double Psea = 101325;


    void reset(void);
    void readPROM(void);

    uint16_t readRegister16(uint8_t reg);
    uint32_t readRegister24(uint8_t reg);


//taylor
        double C0, C1, C2, C3, C4, C5 ;

    
};

extern MS5611 ms5611;

#endif
