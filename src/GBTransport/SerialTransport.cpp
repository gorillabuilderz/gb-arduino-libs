#include <Arduino.h>
#include "SerialTransport.h"

SerialTransport::SerialTransport(long baudRate, unsigned long timeout)
		: Transport(timeout) {
	_baudRate = baudRate;
}

bool SerialTransport::initialise() {
	Serial.begin(_baudRate);
	return true;
}

long SerialTransport::getBaudRate() {
	return _baudRate;
}
    
size_t SerialTransport::write(uint8_t command) {
	return Serial.write(command);
}

int SerialTransport::available() {
	return Serial.available();
}

int SerialTransport::read() {
	waitForData();
        
    return Serial.read();
}
