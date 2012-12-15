#include <Arduino.h>
#include "GBIMAC.h"
#include <stdio.h>

#include <SPI.h>

GBIMAC::GBIMAC(int chipSelect)
{
  	_chipSelect = chipSelect;
	initSPI();  
}

GBIMAC::GBIMAC()
{
	_chipSelect = DEFAULT_MAC_CHIP_SELECT;
	initSPI();
}

void GBIMAC::initSPI() 
{
	pinMode(_chipSelect, OUTPUT);
	//disable device
	digitalWrite (_chipSelect, HIGH); 
  
	SPI.begin(); 
	#if defined(__AVR__)
	 SPI.setClockDivider(SPI_CLOCK_DIV32);  
	#else
	 SPI.setClockDivider(168);  
	#endif
}

void GBIMAC::read(byte macAddressArray[]) 
{
	// Select the SPI device  
	digitalWrite (_chipSelect, LOW);
  
	// transmit read opcode
	SPI.transfer(3); 
	// Send read address
	SPI.transfer(0xFA);

	// Read all the bytes in
	for(int index = 0; index < MAC_LENGTH; index++) {
		macAddressArray[index] = SPI.transfer(0xff) & 0xFF;
	}
	
	// Release the SPI device
	digitalWrite (_chipSelect, HIGH);	
}

int GBIMAC::getChipSelect() {
	return _chipSelect;
}

char *GBIMAC::format(uint8_t macAddress[]) {
	char *formattedMac = "00:00:00:00:00:00";
	sprintf(formattedMac, "%02X:%02X:%02X:%02X:%02X:%02X", 
		macAddress[0],
		macAddress[1],
		macAddress[2],
		macAddress[3],
		macAddress[4],
		macAddress[5]);
	return formattedMac;
}
