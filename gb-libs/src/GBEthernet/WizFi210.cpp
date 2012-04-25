#include <Arduino.h>
#include <string.h>
#include <stdarg.h>
#include "WizFi210.h"

// Global instance
WizFi210Class WizFi210;

const char *WizFi210Class::COMMAND_TERMINATOR			= "\n";
const char *WizFi210Class::COMMAND_SECTION_TERMINATOR	= "";
const char *WizFi210Class::COMMAND_SEPERATOR			= ",";

const char  MODEM_RESPONSE::RESPONSE_START			= '[';
const char* MODEM_RESPONSE::OK_START				= "[O";
const char* MODEM_RESPONSE::OK 						= "[OK]";
const char* MODEM_RESPONSE::ERROR_START				= "[E";
const char* MODEM_RESPONSE::ERROR 					= "[ERROR]";
const char* MODEM_RESPONSE::ERROR_INVALID_INPUT 	= "[ERROR: INVALID INPUT]";
const char* MODEM_RESPONSE::ERROR_SOCKET_FAILURE 	= "[ERROR: SOCKET FAILURE]";
const char* MODEM_RESPONSE::ERROR_NO_CID 			= "[ERROR: NO CID]";

//const char* MODEM_COMMANDS::AT 						= "AT";


WizFi210Class::WizFi210Class() {
 	_connected = false;
 	_inDataMode = false;
 	_transport = SC16SpiTransport(2, SC16IS740_BAUDRATE.B115200);
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
  
  	sendCommand("AT", COMMAND_TERMINATOR);
	receiveResponse();

	// Disable echo
	sendCommand("ATE0", COMMAND_TERMINATOR);
	receiveResponse();
	
	// Enable numerical responses
	sendCommand("ATV0", COMMAND_TERMINATOR);
	receiveResponse(MODEM_RESPONSE_CODES::OK);
	
  	_transport.enableHardwareFlowControl(true);

  	// Enalbe hardware flow control
  	sendCommand("AT&R1", COMMAND_TERMINATOR);
  	// TODO Hang or continue and return false?
  	return receiveResponse(MODEM_RESPONSE_CODES::OK);
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
		if(DEBUG) Serial.print(varCommand);
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
	if(DEBUG) Serial.print(byte);

  	_transport.select();
  	_transport.prepareWrite();
  	size_t written = _transport.write(byte);
  	_transport.deselect();

  	return written;
}

size_t WizFi210Class::write(const char *string) {
	if(DEBUG) Serial.print(string);
	
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
  		if(DEBUG) { Serial.write(buffer[index]); }
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


bool WizFi210Class::isResponseOK(String response) {
	// Do a couple of quick checks before doing an expensive check
	if(response.indexOf(MODEM_RESPONSE::RESPONSE_START) < 0) {
		return false;
	}
	
	if(response.indexOf(MODEM_RESPONSE::OK_START) < 0) {
		return false;
	}

	return response.indexOf(MODEM_RESPONSE::OK) >= 0;
}

bool WizFi210Class::isResponseError(String response) {
	// Do a couple of quick checks before doing an expensive check
	if(response.indexOf(MODEM_RESPONSE::ERROR_START) < 0) {
		return false;
	}
	
	return response.indexOf(MODEM_RESPONSE::ERROR) >= 0 ||
        			response.indexOf(MODEM_RESPONSE::ERROR_INVALID_INPUT) >= 0 ||
        			response.indexOf(MODEM_RESPONSE::ERROR_SOCKET_FAILURE) >= 0 ||
	        		response.indexOf(MODEM_RESPONSE::ERROR_NO_CID) >= 0;
}

bool WizFi210Class::receiveResponse(char expectedResponse) {
	unsigned long timeout = millis() + TIMEOUT;
  
  	char response = -1;
  	int available;

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
        		if(DEBUG) Serial.print(response);   
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
			if(response == expectedResponse) {
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
    	if(DEBUG) Serial.println(response);
  	}
  	else {
  		if(response >= 0) {
  			if(DEBUG) {
				if(DEBUG) Serial.println();
  				if(MODEM_RESPONSE_CODES::OK) {
    	      		Serial.println("-- OK Received");
  				}
  				else {
					Serial.print("Other response received: ");
					Serial.println(response);
  				}
  			}
  		}
  	}
  
  	return response == expectedResponse;
}

char* WizFi210Class::receiveResponse() {
	unsigned long timeout = millis() + TIMEOUT;
  
  	GString response = "";
  	int available;

  	_transport.select();
  	while(timeout > millis()) {
    	if(available = _transport.available()) {
    		// Extend the timeout, modem is responding
    		timeout = millis() + TIMEOUT;
    		
    		_transport.deselect();
      		_transport.select();
      
      		_transport.prepareRead();
      		for(int index = 0; index < available; index++) {
        		char responseChar = _transport.read();
        		// Need to be careful with mem usage here, the modem could spit out a lot of data
        		response += responseChar;
        		if(DEBUG) Serial.print(responseChar);   
      		}      
      		_transport.deselect();
      
	      	if(response.indexOf(MODEM_RESPONSE::RESPONSE_START) >= 0) {
	        	if(isResponseOK(response)) {
		        	if(DEBUG) Serial.println();
	    	      	if(DEBUG) Serial.println("-- OK Received");
	        	  	break;
	        	}
	        	else if(isResponseError(response)){
	          		if(DEBUG) Serial.println();          
	          		if(DEBUG) Serial.print("-- Received error: ");
	          		if(DEBUG) Serial.println(response);
	          		if(DEBUG) Serial.println("-- !!locking up!!");
	          		while(true);
	        	}
			}
			else {
				// Discard the response
				response = "";
			}
        
      		_transport.select();
		}
	}
  
	if(timeout <= millis()) {
		if(DEBUG) Serial.println();     
    	if(DEBUG) Serial.println("-- Timed Out");
    	if(DEBUG) Serial.println("Response was:");
    	if(DEBUG) Serial.println(response);
  	}
  
  	return response.getBuffer();
}

void WizFi210Class::setMac(uint8_t *mac) {
	sendCommand("AT+NMAC=", COMMAND_SECTION_TERMINATOR);
	writeMAC(mac);
	sendCommand(COMMAND_TERMINATOR);
	receiveResponse(MODEM_RESPONSE_CODES::OK);
}

bool WizFi210Class::setWPAPSK(const char* SSID, const char* passphrase) {
	sendCommand("AT+WPAPSK=", SSID, COMMAND_SEPERATOR, passphrase, COMMAND_TERMINATOR);
  	return receiveResponse(MODEM_RESPONSE_CODES::OK);
}

void WizFi210Class::disassociate() {
  sendCommand("AT+WD", COMMAND_TERMINATOR);
  receiveResponse(MODEM_RESPONSE_CODES::OK);
}

bool WizFi210Class::associate(const char* SSID) {
  	sendCommand("AT+WA=", SSID, COMMAND_TERMINATOR);
  	return receiveResponse(MODEM_RESPONSE_CODES::OK);
}

bool WizFi210Class::setAutoAssociate(const char* SSID) {
  	sendCommand("AT+WAUTO=0,", SSID, COMMAND_TERMINATOR);
	return receiveResponse(MODEM_RESPONSE_CODES::OK);
}

bool WizFi210Class::enableDHCP(bool enable) {
	sendCommand("AT+NDHCP=", enable ? "1" : "0", COMMAND_TERMINATOR);
	return receiveResponse(MODEM_RESPONSE_CODES::OK);
}

bool WizFi210Class::setNetworkParameters(uint8_t *address, uint8_t *netMask, uint8_t *gateway) {
	sendCommand("AT+NSET=", COMMAND_SECTION_TERMINATOR);
	writeIP(address);
	sendCommand(COMMAND_SEPERATOR, COMMAND_SECTION_TERMINATOR);
	writeIP(netMask);
	sendCommand(COMMAND_SEPERATOR, COMMAND_SECTION_TERMINATOR);
	writeIP(gateway);
	sendCommand(COMMAND_TERMINATOR, COMMAND_SECTION_TERMINATOR);

	return receiveResponse(MODEM_RESPONSE_CODES::OK);;
}

void WizFi210Class::enterDataMode() {
	if(_inDataMode) {
		return;
	}

	if(DEBUG) Serial.print("Entering data mode");
	sendCommand("ATO", COMMAND_TERMINATOR);
	_inDataMode = receiveResponse(MODEM_RESPONSE_CODES::OK);
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
	receiveResponse(MODEM_RESPONSE_CODES::OK);
}

void WizFi210Class::setAutoTcpListen(int port) {
  	sendCommand("AT+NAUTO=1,1,,", COMMAND_SECTION_TERMINATOR);
  	print(port);
  	sendCommand(COMMAND_TERMINATOR);
	receiveResponse(MODEM_RESPONSE_CODES::OK);
}

bool WizFi210Class::autoAssociateAndConnect() {
	sendCommand("ATA", COMMAND_TERMINATOR);
	_connected = receiveResponse(MODEM_RESPONSE_CODES::OK);
	_inDataMode = _connected;
	return _connected;
}
 

void WizFi210Class::closeAllConnections() {
	sendCommand("AT+NCLOSEALL", COMMAND_TERMINATOR);
	_connected = !receiveResponse(MODEM_RESPONSE_CODES::OK);
}

bool WizFi210Class::connected() {
	return _connected;
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
