#include "arduino.h"
#include "../definitions.h"
#include "SPI.h"
#include "Variables.h"


class EEPROM
{
  public:

    int readByte(int address);
    void writeByte(int address, int data);
	void printAll(void);
	
};

extern EEPROM eeprom;

template <typename T> int eepromWrite(int ee, const T& value)
{
	const byte* p = (const byte*)(const void*)&value;
	int i;
	for (i = 0; i < sizeof(value); i++)
	eeprom.writeByte(ee++, *p++);
	return i;
}

template <typename T> int eepromRead(int ee, T& value)
{
	byte* p = (byte*)(void*)&value;
	int i;
	for (i = 0; i < sizeof(value); i++)
	*p++ = eeprom.readByte(ee++);
	return i;
}


