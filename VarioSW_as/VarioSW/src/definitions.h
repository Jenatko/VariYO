
#define PCB_VER14_PLUS
//#define EPAPER_V2
#define SERIALDEBUG
//#define WAITATSETUPFORSERIAL
//#define USEINTERNALFLASH

#ifdef PCB_VER14_PLUS
#define POWER_ON 1
#define POWER_OFF 0
#else
#define POWER_ON 0
#define POWER_OFF 1
#endif


#define PA11 0
#define PA10 1
#define PA14 2
#define PA09 3
#define PA08 4
#define PA15 5
#define PA20 6
#define PA21 7
#define PA06 8
#define PA07 9
#define PA18 10
#define PA16 11
#define PA19 12
#define PA17 13
#define PA02 14
#define PB08 15
#define PB09 16
#define PA04 17
#define PA05 18
#define PB02 19
#define PA22 20
#define PA23 21
#define PA12 22
#define PB10 23
#define PB11 24
#define PB03 25
#define PA27 26
#define PA28 27
#define PA24 28
#define PA25 29
#define PB22 30
#define PB23 31
#define PA13 38

#define PA03 42



#define BUZZER_TIMER_FREQ 3e6
#define BAROMETER_TIMER_FREQ 61

#define SI7021_ADDRESS 0x40
#define SI7021_MEASURE_RH 0xF5
#define SI7021_MEAUSRE_TEMP 0xF3
#define SI7021_READ_TEMP_FROM_RH 0xE0



#define EEPROM_INSTR_RDSR 0b101
#define EEPROM_INSTR_WREN 0b110
#define EEPROM_INSTR_WRDI 0b100
#define EEPROM_INSTR_READ 0b11
#define EEPROM_INSTR_WRITE 0b10

#define ENA_GPS 1
#define ENA_GPS_LOW_POWER 2
#define ENA_TRACKLOG 4
#define ENA_BUZZER 8
#define ENA_BUZZ_WHEN_LOGGING 16
#define ENA_HEATER_AUTO 32
#define ENA_HEATER 64

//---------------------------------
//-----GPIOs----------------------
//----------------------------------
#define BUTTON_UP  PB03
#define BUTTON_DOWN PB02
#define BUTTON_LEFT PA28 
#define BUTTON_RIGHT PA27
#define BUTTON_CENTER PB09


#define EEPROM_CS PA20

#define SD_CS PA08
#define SD_DETECT PA07
#define SD_RST PA11


#define DISP_CS PA13
#define DISP_DC PA14
#define DISP_RST PA15
#define DISP_BUSY PA10

#define BUZZER_PIN PA19


#define GPS_CS PA04
#define GPS_BCKP PB08


#define IMU_CS PA05

#define MS5611_SPI_FREQ          1000000
#define BARO_CS PA06


#define SRAM_CS  PA21

#define MISO_PROG PA12
#define MOSI_PROG PB10
#define SCK_PROG PB11

#define MISO_IRQ PA18
#define MOSI_IRQ PA16
#define SCK_IRQ PA17

#define POWER_ENA PA09

#define DAC PA02

#define HEAT PA03

#define BT_UART_TX PB22







#define WIND_RBUFF_SIZE 60

#define TRACKLOG_FILE_CLOSED 0
#define TRACKLOG_FILE_OPEN 1

#define RAD2DEG 57.29578f
#define DEG2RAD  0.01745329f

#define KPH2MPS 0.2777777778f
#define MPS2KPH 3.6f

#define IMU_BIT_PER_G	8192.0f   //for 4G range


#define BMX160_PRESENT 1
#define LPS33_PRESENT 2
#define SI7021_PRESENT 4
#define GPS_PRESENT 8
#define MAX17055_PRESENT 16
#define SD_PRESENT 32
#define FIFO_FRAME_SIZE 21

#define GAUGE_FONT_0 0
#define GAUGE_FONT_1 1
#define GAUGE_FONT_2 2
#define GAUGE_FONT_3 3
#define GAUGE_FONT_MASK 0x03
#define GAUGE_DIGITS_0 0
#define GAUGE_DIGITS_1 4
#define GAUGE_DIGITS_2 8
#define GAUGE_DIGITS_3 12
#define GAUGE_DIGITS_4 16
#define GAUGE_DIGITS_5 20
#define GAUGE_DIGITS_6 24
#define GAUGE_DIGITS_7 28


#define GAUGE_DIGITS_MASK 0x1C
#define GAUGE_ENA 32
#define GAUGE_FRAME 64
#define GAUGE_SHOW_PLUS_SIGN 128
#define GAUGE_VALIDS 256



