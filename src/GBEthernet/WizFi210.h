#ifndef WizFi210_h
#define WizFi210_h

#include <Arduino.h>
#include <SC16SpiTransport.h>
#include <GString.h>

class MODEM_RESPONSE_CODES {
	public:
		static const char OK					= '0';
		
		static const char ERROR					= '1';
		static const char ERROR_INVALID_INPUT	= '2';
		static const char ERROR_SOCKET_FAILURE	= '3';
		static const char ERROR_NO_CID			= '4';
		static const char ERROR_INVALID_CID		= '5';
		static const char ERROR_NOT_SUPPORTED	= '6';
		
		static const char CONNECTION_SUCCESS	= '7';
		static const char CONNECTION_DISCONNECT	= '8';
		static const char DISASSOCIATED			= '9';
		static const char DISASSOCIATION_EVENT	= 'A';

		bool static isValidResponseCode(uint8_t newByte) {
			return
					newByte == OK ||
					newByte == ERROR ||
					newByte == ERROR_INVALID_INPUT ||
					newByte == ERROR_SOCKET_FAILURE ||
					newByte == ERROR_NO_CID ||
					newByte == ERROR_INVALID_CID ||
					newByte == ERROR_NOT_SUPPORTED ||
					newByte == CONNECTION_SUCCESS ||
					newByte == CONNECTION_DISCONNECT ||
					newByte == DISASSOCIATED ||
					newByte == DISASSOCIATION_EVENT;
		}
};

class MODEM_COMMANDS {
	public:
		static const char *AT;
};

/**
 * Class deals with modem responses. It builds up received bytes and looks for specific response arrays.
 *
 * We're looking for reponse of: \n0
 *
 */
class ResponseCodeHandler {
public:
	void reset() {
		_byteCount = 0;
	}

	void putByte(uint8_t newByte) {
		// If the new byte is carriage return and byteCount is zero, this is the start of the response
		if(handleNumericalResponse(newByte) ||
			(_byteCount == 1 && MODEM_RESPONSE_CODES::isValidResponseCode(newByte))
		) {
			// Add the new byte and increment byte count
			_responseBuffer[_byteCount++] = newByte;
		}
		else {
			// Force a reset, don't have the response we want in the order we want
			reset();
		}
	}

	bool isResponseReady() {
		// Is ready if the byte count is 2
		return _byteCount == 2;
	}

	uint8_t getResponseCode() {
		if(isResponseReady()) {
			return _responseBuffer[1];
		}
		else {
			return -1;
		}
	}

private:
	char _responseBuffer[5];
	uint8_t _byteCount;

	/* Not currently used. Needs to be tested to work as expected */
	bool handleTextResponse(uint8_t newByte) {
		return 	(newByte == '\r' && _byteCount == 0) ||
				(newByte == '\n' && _byteCount == 1) ||
				(_byteCount == 2) ||
				(newByte == '\r' && _byteCount == 3) ||
				(newByte == '\n' && _byteCount == 4);
	}

	bool handleNumericalResponse(uint8_t newByte) {
		return newByte == 0x0A && _byteCount == 0;
	}
};

class WizFi210Class : public Stream
{
  public:
  	static const char *COMMAND_TERMINATOR;
  	static const char *COMMAND_SECTION_TERMINATOR;
  	static const char *COMMAND_SEPERATOR;

    WizFi210Class(uint8_t resetPin = RESET_PIN, uint8_t chipSelectPin = DEFAULT_CHIP_SELECT,
    		uint8_t associatePin = N_ASSOCIATE, uint8_t wifiOkPin = N_WIFI_OK);
    
    size_t write(const char *string);
    size_t write(const uint8_t byte);
    size_t write(const uint8_t *buffer, size_t size);
    int available();
    int read();
    int peek() { return 0; };
    void flush() {};
    
    void reset();
    bool initialise();
    void sendCommand(const char *command, ...);
    bool connected();
    bool associated();

    char receiveResponse(char expectedResponse = -1);
    bool isOk(char response);
    
    // Keep things transparent to the application
    void setMac(uint8_t *mac);
    bool setWPAPSK(const char* SSID, const char* passphrase);
    bool associate(String SSID);
    bool associate(const char* SSID);
    bool setAutoAssociate(String SSID);
    bool setAutoAssociate(const char* SSID);
    void disassociate();
    bool enableDHCP(bool enable);
    bool setNetworkParameters(uint8_t *address, uint8_t *netMask, uint8_t *gateway);
    void dnsLookup();
    void setAutoTcpConnect(uint8_t *address, int port);
    void setAutoTcpListen(int port);
    bool autoAssociateAndConnect();
    void tcpConnect(uint8_t *address, int port);
    void udpConnect(uint8_t *address, int port);
    void udpServer(int port);
    
    void closeAllConnections();
    void enterDataMode();
    void escapeDataMode();

    Transport* getTransport();
    uint8_t getResetPin();
	
  private:
  	static const bool DEBUG 					= true;
  	static const uint8_t RESET_PIN 				= A1;
  	static const uint8_t DEFAULT_CHIP_SELECT 	= 2;
  	static const uint8_t N_ASSOCIATE   			= 5;
  	static const uint8_t N_WIFI_OK     			= 6;
  	static const unsigned long TIMEOUT 			= 15000;

  	uint8_t _resetPin;
  	uint8_t _associatePin;
  	uint8_t _wifiOkPin;

  	ResponseCodeHandler _responseHandler;
  	SC16SpiTransport _transport;
  	
  	void writeIP(uint8_t *ip);
  	void writeMAC(uint8_t *mac);
};

extern WizFi210Class WizFi210;

#endif
