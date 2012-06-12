#include <Arduino.h>
#include <string.h>
#include <stdarg.h>
#include "WizFi210.h"

// Global instance
WizFi210Class WizFi210;

const char *WizFi210Class::COMMAND_TERMINATOR			= "\n";
const char *WizFi210Class::COMMAND_SECTION_TERMINATOR	= "";
const char *WizFi210Class::COMMAND_SEPERATOR			= ",";

WizFi210Class::WizFi210Class() {
 	_connected = false;
 	_inDataMode = false;
 	_transport = SC16SpiTransport(DEFAULT_CHIP_SELECT, SC16IS740_BAUDRATE.B115200);
 	pinMode(N_ASSOCIATE, INPUT);
 	pinMode(N_WIFI_OK, INPUT);
}

void WizFi210Class::reset() {
	if(DEBUG) Serial.println("Modem reset");
		  
  	pinMode(RESET_PIN, OUTPUT);
  	digitalWrite(RESET_PIN, HIGH);    
  	digitalWrite(RESET_PIN, LOW);  
  	delay(200);
  	digitalWrite(RESET_PIN, HIGH);    
  	delay(7000);
}

bool WizFi210Class::initialise() {
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

	// Disable echo
//	sendCommand("ATE0", COMMAND_TERMINATOR);
//	receiveResponse();
	
  	_transport.enableHardwareFlowControl(true);

  	// Enalbe hardware flow control
  	sendCommand("AT&R1", COMMAND_TERMINATOR);
  	// TODO Hang or continue and return false?
  	return isOk(receiveResponse());
}

void WizFi210Class::sendCommand(const char *command, ...) {
	va_list commands;

	bool wasInDataMode = _inDataMode;
	// If where in datamode, disable first
	if(_inDataMode) {
		escapeDataMode();
	}
	
  	_transport.select();
  	_transport.prepareWrite();

	const char *varCommand;
	va_start(commands, command);
  	// Loop through and write out the varargs
	for (varCommand = command; varCommand != COMMAND_SECTION_TERMINATOR; varCommand = va_arg(commands, char*)) {
//		if(DEBUG) Serial.print(varCommand);
		_transport.write(varCommand);

		if(varCommand == COMMAND_TERMINATOR) {
			break;
		}
	}
	va_end(commands);
  	_transport.deselect();
  	
  	// If we were in datamode, get back into it
  	if(wasInDataMode) {
  		enterDataMode();
  	}  
}

size_t WizFi210Class::write(const uint8_t byte) {
//	if(DEBUG) Serial.print(byte);

  	_transport.select();
  	_transport.prepareWrite();
  	size_t written = _transport.write(byte);
  	_transport.deselect();

  	return written;
}

size_t WizFi210Class::write(const char *string) {
//	if(DEBUG) Serial.print(string);
	
  	_transport.select();
  	_transport.prepareWrite();
  	size_t written = _transport.write(string);
  	_transport.deselect();

  	return written;
}

size_t WizFi210Class::write(const uint8_t *buffer, size_t size) {
  	for(int index = 0; index < size; index++) {
//  		Serial.print(buffer[index], HEX);
		// TODO Is not printing bytes quite correctly
//  		if(DEBUG) { Serial.write(buffer[index]); }
  		write(buffer[index]);
  	}

  	return size;
}

int WizFi210Class::available() {
	int available;
	_transport.select();
	available = _transport.available();
	_transport.deselect();
	return available;
}

int WizFi210Class::peek() {
}

int WizFi210Class::read() {
	int read;
	_transport.select();
    _transport.prepareRead();
	read = _transport.read();
	_transport.deselect();
	return read;      
}

bool WizFi210Class::isOk(char response) {
	return response == MODEM_RESPONSE_CODES::OK;
}

char WizFi210Class::receiveResponse() {
	char expectedResponse = MODEM_RESPONSE_CODES::OK;
	unsigned long timeout = millis() + TIMEOUT;
  
  	char response = -1;
  	int available;
  	_responseHandler.reset();

  	_transport.select();
  	while(timeout > millis()) {
    	if(available = _transport.available()) {
    		// Extend the timeout, modem is responding
    		timeout = millis() + TIMEOUT;
    		
    		_transport.deselect();
      		_transport.select();
      
      		_transport.prepareRead();
      		for(int index = 0; index < available; index++) {
        		response = _transport.read();
        		_responseHandler.putByte(response);
        		if(DEBUG) Serial.print(response);
//        		Serial.print(response, HEX);
//        		Serial.print(" ");
      		}      
      		_transport.deselect();

      		_transport.select();
    		// If there's more items available, this response is not what we're looking for, we want the
    		// last byte transmitted
    		if(_transport.available() > 0) {
    			response = -1;
    		}
    		_transport.deselect();
      		// If the response is as expected
			if(_responseHandler.isResponseReady()) {
				break;
			}
			      		
      		_transport.select();
		}
	}
	
	if(DEBUG) Serial.println();

	if(timeout <= millis()) {
		if(DEBUG) Serial.println();     
    	if(DEBUG) Serial.println("-- Timed Out");
    	if(DEBUG) Serial.print("Response was:");
    	if(DEBUG) Serial.println(_responseHandler.getResponseCode());
  	}
  	else {
  		if(_responseHandler.getResponseCode() >= 0) {
  			if(DEBUG) {
				if(DEBUG) Serial.println();
  				if(isOk(_responseHandler.getResponseCode())) {
    	      		Serial.println("-- OK Received");
  				}
  				else {
					Serial.print("Other response received: ");
					Serial.println(response);
  				}
  			}
  		}
  	}
  
  	return response;
}

void WizFi210Class::setMac(uint8_t *mac) {
	sendCommand("AT+NMAC=", COMMAND_SECTION_TERMINATOR);
	writeMAC(mac);
	sendCommand(COMMAND_TERMINATOR);
	receiveResponse();
}

bool WizFi210Class::setWPAPSK(const char* SSID, const char* passphrase) {
	sendCommand("AT+WPAPSK=", SSID, COMMAND_SEPERATOR, passphrase, COMMAND_TERMINATOR);
  	return isOk(receiveResponse());
}

void WizFi210Class::disassociate() {
  sendCommand("AT+WD", COMMAND_TERMINATOR);
  receiveResponse();
}

bool WizFi210Class::associate(const char* SSID) {
  	sendCommand("AT+WA=", SSID, COMMAND_TERMINATOR);
  	receiveResponse();
  	sendCommand("AT", COMMAND_TERMINATOR);
  	receiveResponse();
  	return associated();
}

bool WizFi210Class::setAutoAssociate(const char* SSID) {
  	sendCommand("AT+WAUTO=0,", SSID, COMMAND_TERMINATOR);
	return isOk(receiveResponse());
}

bool WizFi210Class::enableDHCP(bool enable) {
	sendCommand("AT+NDHCP=", enable ? "1" : "0", COMMAND_TERMINATOR);
	return isOk(receiveResponse());
}

bool WizFi210Class::setNetworkParameters(uint8_t *address, uint8_t *netMask, uint8_t *gateway) {
	sendCommand("AT+NSET=", COMMAND_SECTION_TERMINATOR);
	writeIP(address);
	sendCommand(COMMAND_SEPERATOR, COMMAND_SECTION_TERMINATOR);
	writeIP(netMask);
	sendCommand(COMMAND_SEPERATOR, COMMAND_SECTION_TERMINATOR);
	writeIP(gateway);
	sendCommand(COMMAND_TERMINATOR, COMMAND_SECTION_TERMINATOR);

	return isOk(receiveResponse());
}

void WizFi210Class::enterDataMode() {
	if(_inDataMode) {
		return;
	}

	if(DEBUG) Serial.print("Entering data mode");
	sendCommand("ATO", COMMAND_TERMINATOR);
	_inDataMode = isOk(receiveResponse());
}

void WizFi210Class::escapeDataMode() {
	if(!_inDataMode) {
		return;
	}

	if(DEBUG) Serial.println("Escaping data mode");
	write("+++");
	// No new line, so put new line for logging purposes
	if(DEBUG) Serial.println();
	// Delay of one second to exit data mode
	delay(2000);
	_inDataMode = false;
	read();
	write(COMMAND_TERMINATOR);
	read();
	write(COMMAND_TERMINATOR);
	read();
	write(COMMAND_TERMINATOR);
	read();

  	sendCommand("AT", COMMAND_TERMINATOR);
	receiveResponse();
  	sendCommand("AT", COMMAND_TERMINATOR);
	receiveResponse();
}

void WizFi210Class::setAutoTcpConnect(uint8_t *address, int port) {
  	sendCommand("AT+NAUTO=0,1,", COMMAND_SECTION_TERMINATOR);
  	writeIP(address),
  	sendCommand(COMMAND_SEPERATOR, COMMAND_SECTION_TERMINATOR);
  	print(port, DEC);
  	sendCommand(COMMAND_TERMINATOR);
	receiveResponse();
}

void WizFi210Class::setAutoTcpListen(int port) {
  	sendCommand("AT+NAUTO=1,1,,", COMMAND_SECTION_TERMINATOR);
  	print(port);
  	sendCommand(COMMAND_TERMINATOR);
	receiveResponse();
}

bool WizFi210Class::autoAssociateAndConnect() {
	sendCommand("ATA", COMMAND_TERMINATOR);
	_connected = isOk(receiveResponse());
	_inDataMode = _connected;
	return _connected;
}
 

void WizFi210Class::closeAllConnections() {
	sendCommand("AT+NCLOSEALL", COMMAND_TERMINATOR);
	_connected = !isOk(receiveResponse());
}

bool WizFi210Class::connected() {
	return digitalRead(N_WIFI_OK) == 0;
}

void WizFi210Class::writeIP(uint8_t *ip) {
	print(ip[0], DEC);
	write(".");
	print(ip[1], DEC);
	write(".");
	print(ip[2], DEC);
	write(".");
	print(ip[3], DEC);
}

void WizFi210Class::writeMAC(uint8_t *mac) {
	print(mac[0], HEX);
	write(":");
	print(mac[1], HEX);
	write(":");
	print(mac[2], HEX);
	write(":");
	print(mac[3], HEX);
	write(":");
	print(mac[4], HEX);
	write(":");
	print(mac[5], HEX);
}

uint8_t WizFi210Class::getResetPin() {
	return RESET_PIN;
}

Transport *WizFi210Class::getTransport() {
	return &_transport;
}

bool WizFi210Class::associated() {
	return digitalRead(N_ASSOCIATE) == 0;
}

void WizFi210Class::tcpConnect(uint8_t *address, int port) {
  	sendCommand("AT+NCTCP=", COMMAND_SECTION_TERMINATOR);
  	writeIP(address),
  	sendCommand(COMMAND_SEPERATOR, COMMAND_SECTION_TERMINATOR);
  	print(port, DEC);
  	sendCommand(COMMAND_TERMINATOR);
	receiveResponse();
}
