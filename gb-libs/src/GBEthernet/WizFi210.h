#ifndef WizFi210_h
#define WizFi210_h

#include <Arduino.h>
#include <SC16SpiTransport.h>

/**
 * GorillaBuilderz utility class, created primarily to expose the underlying Arduino char array backing
 * the String object
 */
class GString :  public String {
	public:
		GString( const char *value = "" ) : String(value) {};
	    GString( const String &value ) : String(value) {};
	    GString( const char value) : String(value) {};
	    GString( const unsigned char value) : String(value) {};
	    GString( const int value, const int base=10) : String(value, base) {};
	    GString( const unsigned int value, const int base=10 ) : String(value, base) {};
	    GString( const long value, const int base=10 ) : String(value, base) {};
	    GString( const unsigned long value, const int base=10 ) : String(value, base) {};
	
		const GString & operator = ( const GString &rhs ) { return String::operator=(rhs); };
    	const GString & operator +=( const GString &rhs ) { return String::operator+=(rhs); };
    	const GString & operator +=( const char rhs) { return String::operator+=(rhs); };
	
		char *getBuffer() { return buffer; }
};

// TODO Cleanup these. Probably won't need them after using response codes
class MODEM_RESPONSE {
	public:
		static const char  RESPONSE_START;
		static const char* OK;
		static const char* OK_START;
		static const char* ERROR;
		static const char* ERROR_START;
		static const char* ERROR_INVALID_INPUT;
		static const char* ERROR_SOCKET_FAILURE;
		static const char* ERROR_NO_CID;
};

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
};

class MODEM_COMMANDS {
	public:
		static const char *AT;
};

class WizFi210Class : public Stream
{
  public:
    WizFi210Class();
    
    void reset();
    bool initialise();
    void sendCommand(const char *command, ...);
    size_t write(const char *string);
    size_t write(const uint8_t byte);
    size_t write(const uint8_t *buffer, size_t size);
    bool connected();
	int available();
	int read();
	int peek();
	void flush() {};
    
    char* receiveResponse(); 
    bool receiveResponse(char expectedResponse);
    
    
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
    
    // 
    void closeAllConnections();
    void enterDataMode();
    void escapeDataMode();
	
  private:
  	static const char *COMMAND_TERMINATOR;
  	static const char *COMMAND_SECTION_TERMINATOR;
  	static const char *COMMAND_SEPERATOR;
  	static const bool DEBUG 				= true;
  	static const uint8_t RESET_PIN 			= 6;
  	static const unsigned long TIMEOUT 		= 15000;
  
  	SC16SpiTransport _transport;
  	bool _inDataMode;
  	bool _connected;
  	
  	bool isResponseOK(String response);
  	bool isResponseError(String response);
  	void writeIP(uint8_t *ip);
  	void writeMAC(uint8_t *mac);
};

extern WizFi210Class WizFi210;

#endif
