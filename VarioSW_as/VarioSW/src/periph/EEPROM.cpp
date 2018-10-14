#include "EEPROM.h"
#include "arduino.h"
#include "roundbuff.h"



int EEPROM::read(int address) {
  int i;
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  do {
    digitalWrite(EEPROM_CS, 0);
    SPI.transfer(EEPROM_INSTR_RDSR);
    i = SPI.transfer(0) & 1;
    digitalWrite(EEPROM_CS, 1);
  } while (i == 1);

  i = address >> 8;

  digitalWrite(EEPROM_CS, 0);
  //  SPI.transfer(i);
  SPI.transfer(EEPROM_INSTR_READ | (i << 3));
  SPI.transfer(address & 0xff);
  i = SPI.transfer(0) & 1;
  digitalWrite(EEPROM_CS, 1);


  return i;

}

void EEPROM::write(int address, int data) {

  int i;

  do {
    digitalWrite(EEPROM_CS, 0);
    SPI.transfer(EEPROM_INSTR_RDSR);
    i = SPI.transfer(0) & 1;
    digitalWrite(EEPROM_CS, 1);
  } while (i == 1);

  i = address >> 8;

  digitalWrite(EEPROM_CS, 0);
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(EEPROM_INSTR_WREN);
  digitalWrite(EEPROM_CS, 1);

  digitalWrite(EEPROM_CS, 0);
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(EEPROM_INSTR_WRITE | (i << 3));
  SPI.transfer(address & 0xff);
  SPI.transfer(data);
  digitalWrite(EEPROM_CS, 1);

  digitalWrite(EEPROM_CS, 0);
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(EEPROM_INSTR_WRDI);
  digitalWrite(EEPROM_CS, 1);


}

