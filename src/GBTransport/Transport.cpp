#include "Transport.h"

Transport::Transport(unsigned long timeout) {
	_timeout = timeout;
}

bool Transport::initialise() {
	return true;
}

size_t Transport::write(const uint8_t *buffer, size_t size) {
	size_t numberWritten = 0;

   	for(int index = 0; index < size; index++) {
   		numberWritten += write((uint8_t)buffer[index]);
   	}

   	return numberWritten;
}

bool Transport::waitForData() {
	unsigned long timeout = millis() + _timeout;

	while (timeout > millis()) {
		if(available()) {
			return true;
		}
	}
	return false;
}

unsigned long Transport::getTimeout() {
    return _timeout;
}

int16_t Transport::available() {
	return 1;
}
