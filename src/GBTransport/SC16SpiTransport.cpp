#include "SC16SpiTransport.h"
#include <string.h>

SC16IS740_REGISTERS_STRUCT REGISTERS;

SC16SpiTransport::SC16SpiTransport(int chipSelect, int16_t baudRate) {
	_chipSelect = chipSelect;
	_baudRate = baudRate;
}

bool SC16SpiTransport::initialise() {
	pinMode(_chipSelect, OUTPUT);
	deselect();

	SPI.begin();
	setBaudRate(_baudRate);
  	selectAndWriteRegister(REGISTERS.LCR, DEFAULT_LCR);  // 8 data bit, 1 stop bit, no parity
  	clearFIFO();
  	
 	// Perform read/write test to check if UART is working
  	selectAndWriteRegister(REGISTERS.SPR,'H');
  	return selectAndReadRegister(REGISTERS.SPR) == 'H';
}
    
void SC16SpiTransport::clearFIFO() {
  	//selectAndWriteRegister(REGISTERS.FCR, 0x06); // reset TXFIFO, reset RXFIFO, non FIFO mode
	selectAndWriteRegister(REGISTERS.FCR, 0x07); // reset TXFIFO, reset RXFIFO, FIFO mode
}

size_t SC16SpiTransport::write(uint8_t command) {
	SPI.transfer(command);
	return 1;
}

void SC16SpiTransport::prepareRead() {
	SPI.transfer(REGISTERS.RHR | 0x80);
}

void SC16SpiTransport::prepareWrite() {
	SPI.transfer(REGISTERS.THR & 0x7F);
}

void SC16SpiTransport::select() {
	digitalWrite(_chipSelect, LOW);
}

void SC16SpiTransport::deselect() {
	digitalWrite(_chipSelect, HIGH);
}

void SC16SpiTransport::selectAndWriteRegister(uint8_t address, uint8_t data) {
	select();
	writeRegister(address, data);
	deselect();
}

void SC16SpiTransport::writeRegister(uint8_t address, uint8_t data) {
	SPI.transfer(address & 0x7F); // Write command
	SPI.transfer(data);
}

int SC16SpiTransport::read() {
	return SPI.transfer(0x00);
}

int SC16SpiTransport::available() {
	// Read the LSR and return the first bit result
	//return readRegister(REGISTERS.LSR) & 0x01;
	return readRegister(REGISTERS.RXLVL);
}

uint8_t SC16SpiTransport::selectAndReadRegister(uint8_t spiRegister) {
	select();
	uint8_t data = readRegister(spiRegister);
	deselect();
	
	return data; // receive the read data    
}

uint8_t SC16SpiTransport::readRegister(uint8_t spiRegister) {
	SPI.transfer(spiRegister | 0x80); // Send Register, Set Write Bit
	return SPI.transfer(0); // dummy data is sent for spi read
}

void SC16SpiTransport::setChipSelect(int chipSelect) {
	_chipSelect = chipSelect;
}

int SC16SpiTransport::getChipSelect() {
	return _chipSelect;
}

void SC16SpiTransport::setBaudRate(int16_t baudRate) {
	_baudRate = baudRate;
	// Disable Sleep Mode
//	selectAndWriteRegister(REGISTERS.IER, 0x10);  // Disable sleep mode before programming baud
	selectAndWriteRegister(REGISTERS.LCR, 0x80);  // 0x80 to program baudrate
  	selectAndWriteRegister(REGISTERS.DLL, _baudRate & 0xFF);  // 0x60=9600, 0x30=19.2K, 0x08 =115.2K with X1=14.7456 MHz
  	selectAndWriteRegister(REGISTERS.DLM, _baudRate >> 8 & 0xFF);  // divisor = 0x0008 for 115200 bps
}

int16_t SC16SpiTransport::getBaudRate() {
	return _baudRate;
}

void SC16SpiTransport::enableHardwareFlowControl(bool enable) {
	if(enable) {
	  	selectAndWriteRegister(REGISTERS.LCR, 0xBF);  			// access EFR register
	  	selectAndWriteRegister(REGISTERS.EFR, DEFAULT_EFR);  	// enable enhanced registers
	  	selectAndWriteRegister(REGISTERS.MCR, 0x04);  			// 0000 0100 Enable TCR & TLR
		selectAndWriteRegister(REGISTERS.TCR, 0x4F);  			// 16 resume, 32 halt == 48
		selectAndWriteRegister(REGISTERS.EFR, 0xD0);  			// enable Auto CTS, Auto RTS &  enhanced registers
		selectAndWriteRegister(REGISTERS.LCR, DEFAULT_LCR);  	// 8 data bit, 1 stop bit, no parity
		clearFIFO();
	}
	else {
		selectAndWriteRegister(REGISTERS.LCR, 0xBF);  			// access EFR register
		selectAndWriteRegister(REGISTERS.EFR, DEFAULT_EFR);  	// enable enhanced registers
		selectAndWriteRegister(REGISTERS.LCR, DEFAULT_LCR); 	// 8 data bit, 1 stop bit, no parity
		clearFIFO();
	}
}
