#include <Arduino.h>
#include <string.h>
#include <stdarg.h>
#include "WizFi210.h"

const char *WizFi210::COMMAND_TERMINATOR			= "\n";
const char *WizFi210::COMMAND_SECTION_TERMINATOR	= "";
const char *WizFi210::COMMAND_SEPERATOR			= ",";
WizFi210 *WizFi210::_instance;

WizFi210::WizFi210(uint8_t resetPin, uint8_t chipSelectPin, uint8_t associatePin, uint8_t wifiOkPin)
	: _resetPin(resetPin), _associatePin(associatePin), _wifiOkPin(wifiOkPin), _timeout(TIMEOUT) {

 	_transport = SC16SpiTransport(chipSelectPin, SC16IS740_BAUDRATE.B115200);
 	pinMode(_associatePin, INPUT);
 	pinMode(_wifiOkPin, INPUT);
}

WizFi210 *WizFi210::create(uint8_t resetPin, uint8_t chipSelectPin, uint8_t associatePin, uint8_t wifiOkPin) {
	return WizFi210::_instance = new WizFi210(resetPin, chipSelectPin, associatePin, wifiOkPin);
}

WizFi210 *WizFi210::getInstance() {
	return WizFi210::_instance;
}

void WizFi210::reset() {
	if(DEBUG) Serial.println("Modem reset");
		  
  	pinMode(_resetPin, OUTPUT);
  	digitalWrite(_resetPin, HIGH);
  	digitalWrite(_resetPin, LOW);
  	delay(200);
  	digitalWrite(_resetPin, HIGH);
  	delay(7000);
}

bool WizFi210::initialise() {
	reset();
	
	if(!_transport.initialise()) {
		if(DEBUG) Serial.println("Transport init failed");
		return false;
  	}
  	else {
	    if(DEBUG) Serial.println("Transport init success");    
  	}
  	
  	if(DEBUG) Serial.println("Initialising modem");
  
	// Enable numerical responses
	sendCommand("ATV0", COMMAND_TERMINATOR);
	receiveResponse();

	if(!DEBUG) {
		// Disable echo
		sendCommand("ATE0", COMMAND_TERMINATOR);
		receiveResponse();
	}
	
  	_transport.enableHardwareFlowControl(true);

  	// Enalbe hardware flow control
  	sendCommand("AT&R1", COMMAND_TERMINATOR);
  	// Return response result
  	return receiveResponse()->isOk();
}

void WizFi210::sendCommand(const char *command, ...) {
	va_list commands;

  	_transport.select();
  	_transport.prepareWrite();

	const char *varCommand;
	va_start(commands, command);
  	// Loop through and write out the varargs
	for (varCommand = command; varCommand != COMMAND_SECTION_TERMINATOR; varCommand = va_arg(commands, char*)) {
		_transport.write(varCommand);

		if(varCommand == COMMAND_TERMINATOR) {
			break;
		}
	}
	va_end(commands);
  	_transport.deselect();
}

size_t WizFi210::write(const uint8_t byte) {
  	_transport.select();
  	_transport.prepareWrite();
  	size_t written = _transport.write(byte);
  	_transport.deselect();

  	return written;
}

size_t WizFi210::write(const char *string) {
  	_transport.select();
  	_transport.prepareWrite();
  	size_t written = _transport.write(string);
  	_transport.deselect();

  	return written;
}

size_t WizFi210::write(const uint8_t *buffer, size_t size) {
  	for(size_t index = 0; index < size; index++) {
  		write(buffer[index]);
  	}

  	return size;
}

int WizFi210::available() {
	int available;
	_transport.select();
	available = _transport.available();
	_transport.deselect();
	return available;
}

int WizFi210::read() {
	int read;
	_transport.select();
    _transport.prepareRead();
	read = _transport.read();
	_transport.deselect();
	return read;      
}

ResponseCodeHandler *WizFi210::receiveResponse() {
	return receiveResponse(&_numericResponseHandler);
}

ResponseCodeHandler *WizFi210::receiveResponse(ResponseCodeHandler *responseHandler) {
	unsigned long currentTimeout = millis() + _timeout;
  
  	char response = -1;
  	int available;

  	responseHandler->reset();

  	while(currentTimeout > millis()) {
  		_transport.select();

    	if((available = _transport.available())) {
    		// Extend the timeout, modem is responding
    		currentTimeout = millis() + _timeout;
    		
    		_transport.deselect();
      		_transport.select();
      
      		_transport.prepareRead();
      		for(int index = 0; index < available; index++) {
        		response = _transport.read();

        		// NOTE: We are clearing out the buffer before checking for valid response. This is generally ok as the modem
        		//		 always ends with a response. This will become an issue if there is a scenario that meet this requirement
        		responseHandler->putByte(response);
        		if(DEBUG) {
        			Serial.print(response);
//        			Serial.print(response, HEX);
//        			Serial.print(" ");
        		}
      		}      

      		// Deselect preparing for the next available or method exit
    		_transport.deselect();

			// If the response is as expected
			if(responseHandler->isResponseReady()) {
				break;
			}
		}
    	else {
    		_transport.deselect();
    	}
    	// Delay between chip selects appears to make this more stable
    	delayMicroseconds(15);
	}

  	// Lets print the outcome if required
	if(DEBUG) {
		Serial.println();

		if(currentTimeout <= millis()) {
			Serial.println();
			Serial.println("-- Timed Out");
			Serial.print("Response was:");
			Serial.println(response);
		}
		else {
			if(responseHandler->isResponseReady()) {
				Serial.println();

				if(responseHandler->isOk()) {
					Serial.println("-- OK Received");
				}
				else if(responseHandler->isError()) {
					Serial.println("-- ERROR Received");
				}
				else {
					Serial.print("-- Other response received: ");
					Serial.println(responseHandler->getResponse());
				}
			}
		}
	}

	// Reset to the default timeout
  	_timeout = TIMEOUT;

  	return responseHandler;
}

bool WizFi210::wirelessScan() {
	  sendCommand("AT+WS", COMMAND_TERMINATOR);
	  return receiveResponse()->isOk();
}

void WizFi210::setMac(uint8_t *mac) {
	sendCommand("AT+NMAC=", COMMAND_SECTION_TERMINATOR);
	writeMAC(mac);
	sendCommand(COMMAND_TERMINATOR);
	receiveResponse();
}

bool WizFi210::setWPAPSK(const char* SSID, const char* passphrase) {
	sendCommand("AT+WPAPSK=", SSID, COMMAND_SEPERATOR, passphrase, COMMAND_TERMINATOR);
	// A bit longer than standard timeout
	_timeout = 10000;
  	return receiveResponse()->isOk();
}

void WizFi210::disassociate() {
  sendCommand("AT+WD", COMMAND_TERMINATOR);
  receiveResponse();
}

bool WizFi210::associate(const char* SSID) {
	// Enable text response because this command doesn't return a valid numerical response
	sendCommand("ATV1", COMMAND_TERMINATOR);
	receiveResponse(&_textResponseHandler);

  	sendCommand("AT+WA=", SSID, COMMAND_TERMINATOR);
  	// This command can take a bit
  	_timeout = 15000;
  	bool associated = receiveResponse(&_textResponseHandler)->isOk();

  	// Disable text response
	sendCommand("ATV0", COMMAND_TERMINATOR);
	receiveResponse();

  	return associated;
}

bool WizFi210::setAutoAssociate(const char* SSID) {
  	sendCommand("AT+WAUTO=0,", SSID, COMMAND_TERMINATOR);
	return receiveResponse()->isOk();
}

bool WizFi210::enableDHCP(bool enable) {
	sendCommand("AT+NDHCP=", enable ? "1" : "0", COMMAND_TERMINATOR);
	return receiveResponse()->isOk();
}

bool WizFi210::setNetworkParameters(uint8_t *address, uint8_t *netMask, uint8_t *gateway) {
	sendCommand("AT+NSET=", COMMAND_SECTION_TERMINATOR);
	writeIP(address);
	sendCommand(COMMAND_SEPERATOR, COMMAND_SECTION_TERMINATOR);
	writeIP(netMask);
	sendCommand(COMMAND_SEPERATOR, COMMAND_SECTION_TERMINATOR);
	writeIP(gateway);
	sendCommand(COMMAND_TERMINATOR, COMMAND_SECTION_TERMINATOR);

	return receiveResponse()->isOk();
}

void WizFi210::enterDataMode() {
	if(DEBUG) Serial.print("Entering data mode");
	sendCommand("ATO", COMMAND_TERMINATOR);
	receiveResponse()->isOk();
}

void WizFi210::flush() {
  while (available()) read();
}

void WizFi210::escapeDataMode() {
	if(DEBUG) Serial.println("Escaping data mode");
	write("+++\n");
	// No new line, so put new line for logging purposes
	if(DEBUG) Serial.println();
	// Delay of one second to exit data mode
	delay(2000);
	flush();

	sendCommand("AT", COMMAND_TERMINATOR);
	receiveResponse();
}

void WizFi210::setAutoTcpConnect(uint8_t *address, int port) {
  	sendCommand("AT+NAUTO=0,1,", COMMAND_SECTION_TERMINATOR);
  	writeIP(address),
  	sendCommand(COMMAND_SEPERATOR, COMMAND_SECTION_TERMINATOR);
  	print(port, DEC);
  	sendCommand(COMMAND_TERMINATOR);
	receiveResponse();
}

void WizFi210::setAutoTcpListen(int port) {
  	sendCommand("AT+NAUTO=1,1,,", COMMAND_SECTION_TERMINATOR);
  	print(port);
  	sendCommand(COMMAND_TERMINATOR);
	receiveResponse();
}

bool WizFi210::autoAssociateAndConnect() {
	sendCommand("ATA", COMMAND_TERMINATOR);
	// A little bit longer on the timeout for this command
	_timeout = 15000;
	return receiveResponse()->isOk();
}

bool WizFi210::autoConnectExistingAssociation() {
	sendCommand("ATA2", COMMAND_TERMINATOR);
	_timeout = 15000;
	return receiveResponse()->isOk();
}

void WizFi210::closeAllConnections() {
	sendCommand("AT+NCLOSEALL", COMMAND_TERMINATOR);
	receiveResponse()->isOk();
}

bool WizFi210::connected() {
	return digitalRead(_wifiOkPin) == 0;
}

void WizFi210::writeIP(uint8_t *ip) {
	print(ip[0], DEC);
	write(".");
	print(ip[1], DEC);
	write(".");
	print(ip[2], DEC);
	write(".");
	print(ip[3], DEC);
}

void WizFi210::writeMAC(uint8_t *mac) {
	for(int index; index < 6; index++) {
		// Add a leading 0 if 16 or less
		if(mac[index] <= 16) {
			print(0, HEX);
		}

		print(mac[index], HEX);

		if(index < 5) {
			write(":");
		}
	}
}

uint8_t WizFi210::getResetPin() {
	return _resetPin;
}

Transport *WizFi210::getTransport() {
	return &_transport;
}

bool WizFi210::associated() {
	return digitalRead(_associatePin) == 0;
}

bool WizFi210::tcpConnect(uint8_t *address, int port) {
  	sendCommand("AT+NCTCP=", COMMAND_SECTION_TERMINATOR);
  	writeIP(address),
  	sendCommand(COMMAND_SEPERATOR, COMMAND_SECTION_TERMINATOR);
  	print(port, DEC);
  	sendCommand(COMMAND_TERMINATOR);
	return receiveResponse()->isOk();
}
