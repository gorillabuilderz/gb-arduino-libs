#ifndef SpiTransport_h
#define SpiTransport_h

#include <Arduino.h>
#include <Transport.h>
#include <SPI.h>

class SpiTransport : public Transport
{
  public:
    SpiTransport(int chipSelect = 7);
    
    void setChipSelect(int chipSelect);
    int getChipSelect();
    bool initialise();
    size_t write(const uint8_t command);
    size_t write(const char *string);
    int read();
    int available();
	void select();    
    void deselect();    
	
  private:
  	long _clockDivisor;
  	int _chipSelect;
  	uint8_t _readByteOnAvailable;
  	
  	/**
  	 * Reads the input if the readByteOnAvailable is not set
  	 */
  	uint8_t readIfNotByteOnAvailable();
};

#endif
