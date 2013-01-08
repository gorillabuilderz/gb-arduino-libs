#ifndef WizFi210_h
#define WizFi210_h

#include <Arduino.h>
#include <SC16SpiTransport.h>
#include <GString.h>

#ifndef GB_DEBUG
#define GB_DEBUG 	false
#endif

/**
 * Class deals with modem responses. It builds up received bytes and looks for specific response arrays.
 *
 * We're looking for reponse of: \n0 or \n\r[OK]\n\r
 *
 */
class ResponseCodeHandler {
public:
	void reset() {
		_responseBuffer[0] = 0;
		_responseBuffer[1] = 0;
		_responseBuffer[2] = 0;
		_responseBuffer[3] = 0;
		// Clear the possibly is ready trigger
		_responseBuffer[_byteCount] = 0;
		_byteCount = 0;
	}

	void putByte(uint8_t newByte) {
		// If the response is ready exit
		if(isResponseReady()) return;

		// Add the new byte and increment byte count, preserve the counter
		_responseBuffer[_byteCount] = newByte;

		// Check the buffer state, if it's no good, reset it
		if(isBufferValid()) {
			// If the reponse is ready, don't increment
			if(!isResponseReady() && _byteCount != BUFFER_LENGTH) { // Guard against buffer overflow. Don't care about extra bytes for now
				// Increment the counter
				_byteCount++;
			}
		}
		else {
			// Force a reset, don't have the response we want in the order we want
			reset();
		}
	}

	virtual bool isResponseReady() = 0;
	virtual bool isOk() = 0;
	virtual bool isError() = 0;
	virtual char getResponse() = 0;

protected:
	static const int BUFFER_LENGTH = 35;
	static char _responseBuffer[BUFFER_LENGTH];
	uint8_t _byteCount;
	virtual bool isBufferValid() = 0;
};

class NumericResponseCodeHandler : public ResponseCodeHandler {
public:
	bool isResponseReady() { return _byteCount == 2;	}
	bool isOk() { return isResponseReady() && _responseBuffer[1] == '0'; }
	bool isError() { return isResponseReady() && (_responseBuffer[1] > 0x30 && _responseBuffer[1] < 0x37); }
	char getResponse() { return _responseBuffer[1];	}

protected:
	bool isBufferValid() {
		if(_byteCount == 0 && _responseBuffer[0] != 0x0A) return false;
		// '0' - '9' and 'A'
		if(_byteCount == 1 && ((_responseBuffer[1] < 0x30 || _responseBuffer[1] > 0x39) && _responseBuffer[1] != 'A')) return false;

		return true;
	}
};

class TextResponseCodeHandler : public ResponseCodeHandler {
public:
	bool isResponseReady() { return _byteCount > 5 && _responseBuffer[_byteCount] == 0x0A; }
	bool isOk() { return isResponseReady() && _responseBuffer[3] == 'O'; }
	bool isError() { return isResponseReady() && _responseBuffer[3] == 'E'; }
	char getResponse() { return _responseBuffer[3];	}

protected:
	bool isBufferValid() {
		if(_byteCount == 0 && _responseBuffer[0] != 0x0D) return false;
		if(_byteCount == 1 && _responseBuffer[1] != 0x0A) return false;
		if(_byteCount == 2 && _responseBuffer[2] != '[') return false;
		if(_byteCount == 3 && (_responseBuffer[3] != 'O' && _responseBuffer[3] != 'E' && _responseBuffer[3] != 'C'&& _responseBuffer[3] != 'D')) return false;

		return true;
	}
};

class WizFi210 : public Stream
{
  public:
  	static const char *COMMAND_TERMINATOR;
  	static const char *COMMAND_SECTION_TERMINATOR;
  	static const char *COMMAND_SEPERATOR;

    WizFi210(uint8_t resetPin = RESET_PIN, uint8_t chipSelectPin = DEFAULT_CHIP_SELECT,
    		uint8_t associatePin = N_ASSOCIATE, uint8_t wifiOkPin = N_WIFI_OK);
    
    /**
     * Creates an instance of the class, and stores it for use by other classes by getInstance
     */
    static WizFi210* create(uint8_t resetPin = RESET_PIN, uint8_t chipSelectPin = DEFAULT_CHIP_SELECT,
    		uint8_t associatePin = N_ASSOCIATE, uint8_t wifiOkPin = N_WIFI_OK);
    static WizFi210* getInstance();

    size_t write(const char *string);
    size_t write(const uint8_t byte);
    size_t write(const uint8_t *buffer, size_t size);
    int available();
    int read();
    int peek() { return 0; };
    void flush();
    
    void reset();
    bool initialise();
    void sendCommand(const char *command, ...);
    bool connected();
    bool associated();

    ResponseCodeHandler *receiveResponse();
    ResponseCodeHandler *receiveResponse(ResponseCodeHandler *responseHandler);
    
    // Keep things transparent to the application
    bool wirelessScan();
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
    bool autoConnectExistingAssociation();
    bool tcpConnect(uint8_t *address, int port);
    void udpConnect(uint8_t *address, int port);
    void udpServer(int port);
    
    void closeAllConnections();
    void enterDataMode();
    void escapeDataMode();

    Transport* getTransport();
    uint8_t getResetPin();
	
  private:
  	static const bool DEBUG 					= GB_DEBUG;
  	static const uint8_t RESET_PIN 				= A1;
  	static const uint8_t DEFAULT_CHIP_SELECT 	= 2;
  	static const uint8_t N_ASSOCIATE   			= 5;
  	static const uint8_t N_WIFI_OK     			= 6;
  	static const unsigned long TIMEOUT 			= 3000;

  	// Singleton instance
  	static WizFi210 *_instance;

  	uint8_t _resetPin;
  	uint8_t _associatePin;
  	uint8_t _wifiOkPin;
  	unsigned long _timeout;

  	NumericResponseCodeHandler _numericResponseHandler;
  	TextResponseCodeHandler _textResponseHandler;
  	SC16SpiTransport _transport;
  	
  	void writeIP(uint8_t *ip);
  	void writeMAC(uint8_t *mac);
};

//extern WizFi210Class WizFi210;

#endif
