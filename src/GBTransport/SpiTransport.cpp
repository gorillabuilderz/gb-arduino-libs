#include <Arduino.h>
#include <SpiTransport.h>

SpiTransport::SpiTransport(int chipSelect)
		: Transport() {
	_chipSelect = chipSelect;
	_readByteOnAvailable = 0;
}

bool SpiTransport::initialise() {
	pinMode(_chipSelect, OUTPUT);
	deselect();
	
	SPI.begin();
  	SPI.setClockDivider(SPI_CLOCK_DIV32);//slow SPI bus speed
	return true;
}

void SpiTransport::setChipSelect(int chipSelect) {
	_chipSelect = chipSelect;
}

int SpiTransport::getChipSelect() {
	return _chipSelect;
}

void SpiTransport::select() {
	digitalWrite(_chipSelect, LOW);
}

void SpiTransport::deselect() {
	digitalWrite(_chipSelect, HIGH);
	delay(10);
}
    
size_t SpiTransport::write(uint8_t command) {
	SPI.transfer(command);
	return 1;
}

size_t SpiTransport::write(const char *string) {
	unsigned int length = strlen(string);
	
	for(int index = 0; index < length; index++) {
		SPI.transfer(string[index]);
	}

	return length;
}

uint8_t SpiTransport::readIfNotByteOnAvailable() {
	// If no data waiting to be read
	if(_readByteOnAvailable == 0) {
		// Read it
		_readByteOnAvailable = SPI.transfer(0xFF) & 0xFF;
		// Will be data if not 0xFF, so if 0xFF, no data
		_readByteOnAvailable = _readByteOnAvailable == 0xFF ? 0 : _readByteOnAvailable;
	}
	
	return _readByteOnAvailable;
}

int SpiTransport::available() {
	delay(1);

	readIfNotByteOnAvailable();

	// If some data read (or waiting to be read
 	if(_readByteOnAvailable != 0) {
     	return 1;
    }
    else {
    	return 0;
    }
}

int SpiTransport::read() {
	// Need to put this here to read the last byte if it's not already available
	readIfNotByteOnAvailable();
	
	if(_readByteOnAvailable) {
		// A byte was received on available, return that one
		uint8_t returnByte = _readByteOnAvailable;
		_readByteOnAvailable = 0;
		return returnByte;
	}
	else {
		// Return nothing read
		return _readByteOnAvailable;
	}
}
