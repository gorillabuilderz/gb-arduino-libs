#ifndef SC16SpiTransport_h
#define SC16SpiTransport_h

#include <Arduino.h>
#include <Transport.h>
#include <SPI.h>

struct SC16IS740_REGISTERS_STRUCT {
	static const uint8_t 	 IER        = 0x01 << 3;
	static const uint8_t 	 LCR        = 0x03 << 3;
	static const uint8_t 	 MCR        = 0x04 << 3;
	static const uint8_t 	 LSR        = 0x05 << 3;	
	static const uint8_t 	 FCR        = 0x02 << 3;
	static const uint8_t 	 DLL        = 0x00 << 3;
	static const uint8_t 	 DLM        = 0x01 << 3;
	static const uint8_t 	 EFR        = 0x02 << 3;
	static const uint8_t 	 SPR        = 0x07 << 3;
	static const uint8_t 	 TCR        = 0x06 << 3;	// These registers are accessible only when MCR[2] = 1 and EFR[4] = 1	
	static const uint8_t 	 TLR        = 0x07 << 3;	// These registers are accessible only when MCR[2] = 1 and EFR[4] = 1
	static const uint8_t 	 TXLVL      = 0x08 << 3;
	static const uint8_t 	 RXLVL      = 0x09 << 3;
	static const uint8_t 	 THR        = 0x00 << 3;
	static const uint8_t 	 RHR        = 0x00 << 3;
};

// 0x60=9600, 0x30=19.2K, 0x08 =115.2K with X1=14.7456 MHz
struct SC16IS740_BAUDRATE_STRUCT {
	static const int16_t B9600		= 0x60;
	static const int16_t B19200		= 0x30;
	static const int16_t B115200	= 0x08;
};

extern SC16IS740_BAUDRATE_STRUCT SC16IS740_BAUDRATE;

class SC16SpiTransport : public Transport
{
  public:
    SC16SpiTransport(int chipSelect = 6, int16_t baudRate = SC16IS740_BAUDRATE.B9600);
    
    void setChipSelect(int chipSelect);
    int getChipSelect();
    int16_t getBaudRate();
    void setBaudRate(int baudRate);
    bool initialise();
    size_t write(uint8_t command);
    size_t write(const char *string);
    int read();
    void prepareRead();
    void prepareWrite();
	void select();    
    void deselect();
  	int available();
  	void enableHardwareFlowControl(bool enable);
	
  private:
  	static const unsigned char DEFAULT_LCR = 0x03;
  	static const unsigned char DEFAULT_EFR = 0x01;
  	
  	void clearFIFO();
  	void selectAndWriteRegister(uint8_t address, uint8_t data);
  	uint8_t selectAndReadRegister(uint8_t SPIregister);
  	uint8_t readRegister(uint8_t SPIregister);
  	void writeRegister(uint8_t address, uint8_t data);
	int _chipSelect;
	int16_t _baudRate;
};

#endif
