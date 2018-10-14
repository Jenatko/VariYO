#include <Arduino.h>
#include <GxGDEP015OC1/GxGDEP015OC1.h>
#include "powerModes.h"
#include "definitions.h"
#include "button_control.h"
#include "Interrupt_counter.h"
#include "wiring_private.h"

void powerOff() {
	display.fillRect(0, 0, 199, 199, GxEPD_WHITE);
	display.setCursor(20, 100);
	display.print("Power Off");
	display.update();
	USB->DEVICE.CTRLA.reg &= ~USB_CTRLA_ENABLE;
	SCB->SCR |= 1 << 2;
	EIC->WAKEUP.reg = EIC_WAKEUP_WAKEUPEN2;
	display.powerDown();
	REG_TC4_INTENCLR = TC_INTENCLR_MC1 | TC_INTENCLR_MC0 | TC_INTENCLR_OVF;
	attachInterrupt(BUTTON_LEFT, wakeUp, LOW);
	// pinMode(BUTTON_LEFT, OUTPUT);
	// pinMode(BUTTON_RIGHT, OUTPUT);
	// pinMode(BUTTON_UP, OUTPUT);
	// pinMode(BUTTON_DOWN, OUTPUT);
	// pinMode(BUTTON_CENTER, INPUT_PULLUP);
	/*USB->DEVICE.CTRLA.reg &= ~USB_CTRLA_ENABLE;
	USB->HOST.CTRLA.bit.ENABLE=0;
	USB->DEVICE.CTRLA.bit.ENABLE=0;
	I2S->CTRLA.bit.ENABLE=0;

	ADC->CTRLA.bit.ENABLE=0;

	DAC->CTRLA.bit.ENABLE=0;*/
	allLow();
	__WFI();
}


void allLow() {
	digitalWrite(SD_RST, 0);
	digitalWrite(SD_OE, 1);
	
	digitalWrite(GPS_CS, 0);
	digitalWrite(EEPROM_CS, 0);
	digitalWrite(SRAM_CS, 0);
	digitalWrite(BARO_CS, 0);
	digitalWrite(IMU_CS, 0);
	digitalWrite(SD_CS, 0);	
	pinMode(MISO_PROG, OUTPUT);
	pinMode(MOSI_PROG, OUTPUT);
	pinMode(SCK_PROG, OUTPUT);
	digitalWrite(MISO_PROG, 0);
	digitalWrite(MOSI_PROG, 0);
	digitalWrite(SCK_PROG, 0);
	
		pinMode(MISO_IRQ, OUTPUT);
		pinMode(MOSI_IRQ, OUTPUT);
		pinMode(SCK_IRQ, OUTPUT);
		digitalWrite(MISO_IRQ, 0);
		digitalWrite(MOSI_IRQ, 0);
		digitalWrite(SCK_IRQ, 0);

	


}

void wakeUp() {
	detachInterrupt(BUTTON_LEFT);
	//  pinMode(BUTTON_LEFT, INPUT_PULLUP);
	//  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
	//  pinMode(BUTTON_UP, INPUT_PULLUP);
	//  pinMode(BUTTON_DOWN, INPUT_PULLUP);
	counterInit();
	REG_EIC_INTENCLR = EIC_INTENCLR_EXTINT2;
	REG_EIC_INTFLAG = EIC_INTFLAG_EXTINT2;

	buttons.getButtonPressed();

}

void massStorageEna() {
	display.fillRect(0, 0, 199, 199, GxEPD_WHITE);
	display.setCursor(20, 100);
	display.print("USB mass storage");
	display.setCursor(20, 120);
	display.print("push to end");
	display.update();
	display.powerDown();
	
	pinMode(22, INPUT);
	pinMode(23, INPUT);
	pinMode(24, INPUT);
	pinMode(SD_CS, INPUT);
	digitalWrite(SD_RST, 1);
	digitalWrite(SD_OE, 0);
	while(1)
	if (buttons.getFlag()) {
		if (buttons.getButtonPressed() == 4)
		break;
	}
	digitalWrite(SD_RST, 0);
	delay(100);
	pinPeripheral(22, PIO_SERCOM);
	pinPeripheral(23, PIO_SERCOM);
	pinPeripheral(24, PIO_SERCOM);
	pinMode(SD_CS, OUTPUT);
	digitalWrite(SD_CS, 1);

	digitalWrite(SD_OE, 0);
	
	display.init(0);

	//	display.update();
}


void massStorageDis() {

}