#include <Arduino.h>
#include <string.h>
#include <stdarg.h>
#include "WizFi210.h"

const char *WizFi210::COMMAND_TERMINATOR			= "\n";
const char *WizFi210::COMMAND_SECTION_TERMINATOR	= "";
const char *WizFi210::COMMAND_SEPERATOR			= ",";

WizFi210::WizFi210(uint8_t resetPin, uint8_t chipSelectPin, uint8_t associatePin, uint8_t wifiOkPin)
	: _resetPin(resetPin), _associatePin(associatePin), _wifiOkPin(wifiOkPin) {

 	_transport = SC16SpiTransport(chipSelectPin, SC16IS740_BAUDRATE.B115200);
 	pinMode(_associatePin, INPUT);
 	pinMode(_wifiOkPin, INPUT);
}

WizFi210 *WizFi210::create(uint8_t resetPin, uint8_t chipSelectPin, uint8_t associatePin, uint8_t wifiOkPin) {
	return _instance = new WizFi210(resetPin, chipSelectPin, associatePin, wifiOkPin);
}

WizFi210 *WizFi210::getInstance() {
	return _instance;
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
  	return isOk(receiveResponse());
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

bool WizFi210::isOk(char response) {
	return response == MODEM_RESPONSE_CODES::OK;
}

char WizFi210::receiveResponse(char expectedResponse) {
	unsigned long timeout = millis() + TIMEOUT;
  
  	char response = -1;
  	int available;
  	_responseHandler.reset();

  	_transport.select();
  	while(timeout > millis()) {
    	if((available = _transport.available())) {
    		// Extend the timeout, modem is responding
    		timeout = millis() + TIMEOUT;
    		
    		_transport.deselect();
      		_transport.select();
      
      		_transport.prepareRead();
      		for(int index = 0; index < available; index++) {
        		response = _transport.read();

        		// NOTE: We are clearing out the buffer before checking for valid response. This is generally ok as the modem
        		//		 always ends with a response. This will become an issue if there is a scenario that meet this requirement
        		_responseHandler.putByte(response);
        		if(DEBUG) {
        			Serial.print(response);
//        			Serial.print(response, HEX);
//        			Serial.print(" ");
        		}
      		}      
      		_transport.deselect();

      		_transport.select();

      		// Dodgy logic to cater for when the modem does not send through a \n0 on connect. Instead terminates
      		// command response with a 0 appended. Makes it hard to decipher if the 0 is part of the IP or the OK code
      		// Possibly need to conver this to process a [OK/ERROR] instead

      		// If expected response is set
      		if(expectedResponse != -1) {
				// If there's more items available, this response is not what we're looking for, we want the
				// last byte transmitted
				if(_transport.available() > 0) {
					response = -1;
				}
      		}
    		_transport.deselect();

    		if(expectedResponse == -1) {
				// If the response is as expected
				if(_responseHandler.isResponseReady()) {
					response = _responseHandler.getResponseCode();
					break;
				}
    		}
    		else if(expectedResponse != -1) {
    			if(response == expectedResponse) {
    				break;
    			}
    		}
			      		
      		_transport.select();
		}
	}

  	// Lets print the outcome if required
	if(DEBUG) {
		Serial.println();

		if(timeout <= millis()) {
			Serial.println();
			Serial.println("-- Timed Out");
			Serial.print("Response was:");
			Serial.println(response);
		}
		else {
			if(response >= 0) {
				Serial.println();

				if(isOk(response)) {
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

void WizFi210::setMac(uint8_t *mac) {
	sendCommand("AT+NMAC=", COMMAND_SECTION_TERMINATOR);
	writeMAC(mac);
	sendCommand(COMMAND_TERMINATOR);
	receiveResponse();
}

bool WizFi210::setWPAPSK(const char* SSID, const char* passphrase) {
	sendCommand("AT+WPAPSK=", SSID, COMMAND_SEPERATOR, passphrase, COMMAND_TERMINATOR);
  	return isOk(receiveResponse());
}

void WizFi210::disassociate() {
  sendCommand("AT+WD", COMMAND_TERMINATOR);
  receiveResponse();
}

bool WizFi210::associate(const char* SSID) {
  	sendCommand("AT+WA=", SSID, COMMAND_TERMINATOR);
  	return isOk(receiveResponse(MODEM_RESPONSE_CODES::OK));
}

bool WizFi210::setAutoAssociate(const char* SSID) {
  	sendCommand("AT+WAUTO=0,", SSID, COMMAND_TERMINATOR);
	return isOk(receiveResponse());
}

bool WizFi210::enableDHCP(bool enable) {
	sendCommand("AT+NDHCP=", enable ? "1" : "0", COMMAND_TERMINATOR);
	return isOk(receiveResponse());
}

bool WizFi210::setNetworkParameters(uint8_t *address, uint8_t *netMask, uint8_t *gateway) {
	sendCommand("AT+NSET=", COMMAND_SECTION_TERMINATOR);
	writeIP(address);
	sendCommand(COMMAND_SEPERATOR, COMMAND_SECTION_TERMINATOR);
	writeIP(netMask);
	sendCommand(COMMAND_SEPERATOR, COMMAND_SECTION_TERMINATOR);
	writeIP(gateway);
	sendCommand(COMMAND_TERMINATOR, COMMAND_SECTION_TERMINATOR);

	return isOk(receiveResponse());
}

void WizFi210::enterDataMode() {
	if(DEBUG) Serial.print("Entering data mode");
	sendCommand("ATO", COMMAND_TERMINATOR);
	isOk(receiveResponse());
}

void WizFi210::escapeDataMode() {
	if(DEBUG) Serial.println("Escaping data mode");
	write("+++");
	// No new line, so put new line for logging purposes
	if(DEBUG) Serial.println();
	// Delay of one second to exit data mode
	delay(2000);
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
	return isOk(receiveResponse(MODEM_RESPONSE_CODES::OK));
}
 

void WizFi210::closeAllConnections() {
	sendCommand("AT+NCLOSEALL", COMMAND_TERMINATOR);
	isOk(receiveResponse());
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

uint8_t WizFi210::getResetPin() {
	return _resetPin;
}

Transport *WizFi210::getTransport() {
	return &_transport;
}

bool WizFi210::associated() {
	return digitalRead(_associatePin) == 0;
}

void WizFi210::tcpConnect(uint8_t *address, int port) {
  	sendCommand("AT+NCTCP=", COMMAND_SECTION_TERMINATOR);
  	writeIP(address),
  	sendCommand(COMMAND_SEPERATOR, COMMAND_SECTION_TERMINATOR);
  	print(port, DEC);
  	sendCommand(COMMAND_TERMINATOR);
	receiveResponse();
}
