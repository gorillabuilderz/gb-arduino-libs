#include "Transport.h"

Transport::Transport(unsigned long timeout) {
	_timeout = timeout;
}

bool Transport::initialise() {
	return true;
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

int Transport::available() {
	return 1;
}
