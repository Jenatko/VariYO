/*
  MS5611.cpp - Class file for the MS5611 Barometric Pressure & Temperature Sensor Arduino Library.

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


#include "Arduino.h"


#include <Wire.h>
#include <math.h>

#include "MS5611.h"

MS5611 ms5611;

bool MS5611::begin(ms5611_osr_t osr) {
  Wire.begin();
  Wire.setClock(400000) ;
  reset();
  setOversampling(osr);
  delay(100);
  readPROM();
  return true;
}

long MS5611::getProm(int index) {
  if (index == 6)
    return dT;
  if (index == 7)
    return TEMP2;
  return fc[index];
}

// Set oversampling value
void MS5611::setOversampling(ms5611_osr_t osr)
{
  switch (osr)
  {
    case MS5611_ULTRA_LOW_POWER:
      ct = 1;
      break;
    case MS5611_LOW_POWER:
      ct = 2;
      break;
    case MS5611_STANDARD:
      ct = 3;
      break;
    case MS5611_HIGH_RES:
      ct = 5;
      break;
    case MS5611_ULTRA_HIGH_RES:
      ct = 10;
      break;
  }

  uosr = osr;
}

// Get oversampling value
ms5611_osr_t MS5611::getOversampling(void) {
  return (ms5611_osr_t)uosr;
}

void MS5611::reset(void) {
  SPI_IRQ.beginTransaction(SPISettings(MS5611_SPI_FREQ, MSBFIRST, SPI_MODE0));
  digitalWrite(BARO_CS, 0);
  SPI_IRQ.transfer(MS5611_CMD_RESET);
  digitalWrite(BARO_CS, 1);
  //Wire.write(MS5611_CMD_RESET);

}



void MS5611::readPROM(void) {
  for (uint8_t offset = 0; offset < 6; offset++)  {
    fc[offset] = readRegister16(MS5611_CMD_READ_PROM + (offset * 2));
  }
}



uint32_t MS5611::readRawTemperature(void) {
  requestTemperature();
  delayMicroseconds(ct * 1000);
  return getADCreg();
}

uint32_t MS5611::updatePressure(void) {
  //   requestPressure();
  //  delayMicroseconds(ct * 1000);
  P = pressureCorrection(getADCreg());
  //P = getADCreg();
  return P;
}



uint32_t MS5611::readRawPressure(void) {
  requestPressure();
  delayMicroseconds(ct * 1000);
  return getADCreg();
}

uint32_t MS5611::pressureCorrection(uint32_t pressure) {
  return ((double)pressure * SENS / 2097152 - OFF) / 32768;
}




void MS5611::requestPressure() {
  SPI_IRQ.beginTransaction(SPISettings(MS5611_SPI_FREQ, MSBFIRST, SPI_MODE0));
  digitalWrite(BARO_CS, 0);
  SPI_IRQ.transfer(MS5611_CMD_CONV_D1 + uosr);
  digitalWrite(BARO_CS, 1);

}

void MS5611::requestTemperature() {
  SPI_IRQ.beginTransaction(SPISettings(MS5611_SPI_FREQ, MSBFIRST, SPI_MODE0));
  digitalWrite(BARO_CS, 0);
  SPI_IRQ.transfer(MS5611_CMD_CONV_D2 + uosr);
  digitalWrite(BARO_CS, 1);

}



uint32_t MS5611::getADCreg(void) {
  return readRegister24(MS5611_CMD_ADC_READ);
}



uint32_t MS5611::getRawTemperature(void) {
  return D2;
}

uint32_t MS5611::getRawPressure(void) {
  return D1;
}


int32_t MS5611::readPressure() {
  return P;
}

double MS5611::readTemperature() {
  return ((double)TEMP / 100);
}

double MS5611::readSeaPressure() {
  return Psea;
}

double MS5611::readAltitude(unsigned int pressure) {
  //  SerialUSB.println(Psea);
  // SerialUSB.println(P);
  // SerialUSB.println(TEMP);
  //    SerialUSB.println("");

  //return (double)(pow(Psea / pressure, 1 / 5.257) - 1) * ((double)TEMP / 100 + 273.15) / 0.0065;

  /*
    SerialUSB.println("");
    SerialUSB.println(pressure);
    SerialUSB.println(C0
          + C1 * (pressure - 90000)
          + C2 * (pressure - 90000)* (pressure - 90000)
          + C3 * (pressure - 90000)* (pressure - 90000)* (pressure - 90000)
          + C4 * (pressure - 90000)* (pressure - 90000)* (pressure - 90000)* (pressure - 90000)
          + C5 * (pressure - 90000)* (pressure - 90000)* (pressure - 90000)* (pressure - 90000)* (pressure - 90000));

  */

  return (C0
          + C1 * (pressure - 90000)
          + C2 * (pressure - 90000) * (pressure - 90000)
          + C3 * (pressure - 90000) * (pressure - 90000) * (pressure - 90000)
          + C4 * (pressure - 90000) * (pressure - 90000) * (pressure - 90000) * (pressure - 90000)
          + C5 * (pressure - 90000) * (pressure - 90000) * (pressure - 90000) * (pressure - 90000) * (pressure - 90000));

}


void MS5611::tempUpdate(int stage) {

  /*
    D1 = readRawPressure();

    D2 = readRawTemperature();
    dT = D2 - (uint32_t)fc[4] * 256;



    OFF = fc[1] * 65536L + (int64_t)fc[3] * dT / 128;
    SENS = (int64_t)fc[0] * 32768 + (int64_t)fc[2] * dT / 256;



    TEMP = 2000 + ((int64_t) dT * fc[5]) / 8388608;

    OFF2 = 0;
    SENS2 = 0;
    TEMP2 = 0;
    // SerialUSB.println(Psea);
    //    SerialUSB.println(fc[4]);
    // SerialUSB.println(dT);

    if (TEMP < 2000)
    {
      OFF2 = 5 * ((TEMP - 2000) * (TEMP - 2000)) / 2;
      SENS2 = 5 * ((TEMP - 2000) * (TEMP - 2000)) / 4;
      TEMP2 = (int64_t)dT * dT / 2147483648;
      //   SerialUSB.println(D1);
      //  SerialUSB.println(TEMP2);
      //  SerialUSB.println((long)OFF2);
      //  SerialUSB.println((long)SENS2);

    }

    if (TEMP < -1500)
    {
      OFF2 = OFF2 + 7 * ((TEMP + 1500) * (TEMP + 1500));
      SENS2 = SENS2 + 11 * ((TEMP + 1500) * (TEMP + 1500)) / 2;
    }

    OFF = OFF - OFF2;
    SENS = SENS - SENS2;
    TEMP = TEMP - TEMP2;

    P = (D1 * SENS / 2097152 - OFF) / 32768;


    //  SerialUSB.println((long)P);

  */
  if (stage == 1) {
    D1 = getADCreg();
    requestTemperature();
  }
  else if (stage == 2) {
    D2 = getADCreg();
    dT = D2 - (uint32_t)fc[4] * 256;



    OFF = fc[1] * 65536L + (int64_t)fc[3] * dT / 128;
    SENS = (int64_t)fc[0] * 32768 + (int64_t)fc[2] * dT / 256;



    TEMP = 2000 + ((int64_t) dT * fc[5]) / 8388608;

    OFF2 = 0;
    SENS2 = 0;
    TEMP2 = 0;
    // SerialUSB.println(Psea);
    //    SerialUSB.println(fc[4]);
    // SerialUSB.println(dT);

    if (TEMP < 2000)
    {
      OFF2 = 5 * ((TEMP - 2000) * (TEMP - 2000)) / 2;
      SENS2 = 5 * ((TEMP - 2000) * (TEMP - 2000)) / 4;
      TEMP2 = (int64_t)dT * dT / 2147483648;
      //   SerialUSB.println(D1);
      //  SerialUSB.println(TEMP2);
      //  SerialUSB.println((long)OFF2);
      //  SerialUSB.println((long)SENS2);

    }

    if (TEMP < -1500)
    {
      OFF2 = OFF2 + 7 * ((TEMP + 1500) * (TEMP + 1500));
      SENS2 = SENS2 + 11 * ((TEMP + 1500) * (TEMP + 1500)) / 2;
    }

    OFF = OFF - OFF2;
    SENS = SENS - SENS2;
    TEMP = TEMP - TEMP2;

    P = (D1 * SENS / 2097152 - OFF) / 32768;



    float a_power = pow(Psea, 0.189573);
    //      float a_power = 8.9;

    C0 = (17.6968 * a_power - 153.84) * (TEMP / 100 + 273.15);
    C1 = (-0.000037276 * a_power * (TEMP / 100 + 273.15));
    C2 = (2.46347e-10 * a_power * (TEMP / 100 + 273.15));
    C3 = (-1.99776e-15 * a_power * (TEMP / 100 + 273.15));
    C4 = (1.77e-20 * a_power * (TEMP / 100 + 273.15));
    C5 = (-1.6479e-25 * a_power * (TEMP / 100 + 273.15));

    /*
        SerialUSB.println(a_power);
        SerialUSB.println(C0));
        SerialUSB.println(C1);
        SerialUSB.println(C2);
        SerialUSB.println(C3);
        SerialUSB.println(C4);
        SerialUSB.println(C5);
        SerialUSB.println("");

    */

  }


}


void MS5611::setSeaLevel(int altitude, unsigned int pressure) {
  //  tempUpdate();
  /* SerialUSB.println(readRawPressure());
     SerialUSB.println(TEMP);
       SerialUSB.println((long)OFF);
         SerialUSB.println((long)SENS);
  */
  // P = pressureCorrection(readRawPressure());
  Psea = (double)pressure *  pow((1.0 - 0.0065 * altitude / (TEMP / 100 + 0.0065 * altitude + 273.15)), -5.257)  ;
  //  SerialUSB.print("Psea:");
  //  SerialUSB.println(Psea);
}

void MS5611::putSeaLevel(int pressure) {
  //  tempUpdate();
  /* SerialUSB.println(readRawPressure());
     SerialUSB.println(TEMP);
       SerialUSB.println((long)OFF);
         SerialUSB.println((long)SENS);
  */
  // P = pressureCorrection(readRawPressure());
  Psea = pressure  ;
  // SerialUSB.print("Psea:");
  // SerialUSB.println(Psea);
}


// Read 16-bit from register (oops MSB, LSB)
uint16_t MS5611::readRegister16(uint8_t reg) {
  uint16_t value;

  digitalWrite(BARO_CS, 0);
  SPI_IRQ.beginTransaction(SPISettings(MS5611_SPI_FREQ, MSBFIRST, SPI_MODE0));
  SPI_IRQ.transfer(reg);


  uint8_t vha =   SPI_IRQ.transfer(0);
  uint8_t vla = SPI_IRQ.transfer(0);
  digitalWrite(BARO_CS, 1);


  value = vha << 8 | vla;

  return value;
}

// Read 24-bit from register (oops XSB, MSB, LSB)
uint32_t MS5611::readRegister24(uint8_t reg) {
  uint32_t value;

  digitalWrite(BARO_CS, 0);

  SPI_IRQ.beginTransaction(SPISettings(MS5611_SPI_FREQ, MSBFIRST, SPI_MODE0));
  SPI_IRQ.transfer(reg);

  uint8_t vxa = SPI_IRQ.transfer(0);
  uint8_t vha = SPI_IRQ.transfer(0);
  uint8_t vla = SPI_IRQ.transfer(0);

  digitalWrite(BARO_CS, 1);

  value = ((int32_t)vxa << 16) | ((int32_t)vha << 8) | vla;

  return value;
}
