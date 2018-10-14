#include "arduino.h"
#include "../definitions.h"
#include "SPI.h"


class EEPROM
{
  public:

    int read(int address);
    void write(int address, int data);
};
