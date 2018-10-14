
#define BUZZER_TIMER_FREQ 3e6
#define BAROMETER_TIMER_FREQ 61


#define EEPROM_INSTR_RDSR 0b101
#define EEPROM_INSTR_WREN 0b110
#define EEPROM_INSTR_WRDI 0b100
#define EEPROM_INSTR_READ 0b11
#define EEPROM_INSTR_WRITE 0b10


//---------------------------------
//-----GPIOs----------------------
//----------------------------------
#define BUTTON_UP 42    //PA03
#define BUTTON_DOWN A1  //PA04
#define BUTTON_LEFT A0  //PA02
#define BUTTON_RIGHT A3  //BP08
#define BUTTON_CENTER A2  //PB09

#define EEPROM_CS A4    //PA05

#define SD_CS 9//PA 07
#define SD_DETECT 8//PA06
#define SD_RST 19 //PB02
#define SD_OE 25 //BP03

#define GPS_PPS  4//PA08
//#define GPS_EN  3//PA09

#define DISP_CS 38 //PA13
#define DISP_DC 2//PA14
#define DISP_RST 5//PA15 (SAM_BA bootloader switch)
#define DISP_BUSY 3//PA16

#define BUZZER_PIN 12


#define GPS_CS 0 //PA11
#define GPS_UART_TX 1 //PA10

#define IMU_CS 7//PA21


#define MS5611_SPI_FREQ          1000000
#define BARO_CS          6


//UNUSED
#define UNUSED1 37//PA17
#define UNUSED2 36//PA18
#define UNUSED3 6//PA20
#define UNUSED4 7//PA21


#define SRAM_CS  27 //PA28

#define MISO_PROG 22  //PA12
#define MOSI_PROG 23 //PB10
#define SCK_PROG 24 //PB11

#define MISO_IRQ 10  //PA18
#define MOSI_IRQ 11 //PA16
#define SCK_IRQ 13 //PA17





