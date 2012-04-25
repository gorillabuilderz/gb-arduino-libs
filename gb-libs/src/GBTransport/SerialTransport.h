#ifndef SerialTransport_h
#define SerialTransport_h

#include <Arduino.h>
#include <Transport.h>

class SerialTransport : public Transport
{
  public:
    SerialTransport(long baudRate = 9600, unsigned long timeout = Transport::DEFAULT_TIMEOUT);
    
    bool initialise();
    size_t write(uint8_t command);
    size_t write(const char *string);
    int read();
    int available();
    long getBaudRate();
	
  private:
  	long _baudRate;
};

#endif
